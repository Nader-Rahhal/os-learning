#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct BootSector
{
    uint8_t JumpBoot[3];       // Jump instruction
    uint8_t OEM_Identifier[8]; // OEM identifier
    uint16_t BytesPerSector;   // Bytes per sector (usually 512)
    uint8_t SectorsPerCluster; // Sectors per cluster
    uint16_t ReservedSectors;  // Reserved sectors
    uint8_t NumberOfFATs;      // Number of FATs
    uint16_t RootEntries;      // Root directory entries
    uint16_t TotalSectors;     // Total sectors (if < 65535)
    uint8_t MediaDescriptor;   // Media descriptor
    uint16_t SectorsPerFAT;    // Sectors per FAT
    uint16_t SectorsPerTrack;  // Sectors per track
    uint16_t NumberOfHeads;    // Number of heads
    uint32_t HiddenSectors;    // Hidden sectors
    uint32_t TotalSectorsBig;  // Total sectors (if >= 65535)
    uint8_t DriveNumber;       // Drive number
    uint8_t Reserved1;         // Reserved
    uint8_t BootSignature;     // Extended boot signature
    uint32_t VolumeID;         // Volume serial number
    uint8_t VolumeLabel[11];   // Volume label
    uint8_t FileSystemType[8]; // File system type
    uint8_t BootCode[448];     // Boot code
    uint16_t BootSignature2;   // Boot signature (0xAA55)
};
#pragma pack(pop)

struct FAT12System {
    struct BootSector bootsector;
    struct FileAllocationTable *fat1;
    struct FileAllocationTable *fat2;
    struct Directory *rootdir;
    // struct DataRegion data;
};  

struct FileAllocationTable
{
    uint8_t Entries[4608];
};

struct DataRegion
{
    uint8_t Clusters[512];
};

struct Directory
{
    uint8_t File_Name[8];    // Offset 0
    uint8_t Extension[3];    // Offset 8
    uint8_t Attributes;      // Offset 11
    uint8_t Case;            // Offset 12
    uint8_t CreationTimeMs;  // Offset 13
    uint16_t CreationTime;   // Offset 14
    uint16_t CreationDate;   // Offset 16
    uint16_t LastAccessDate; // Offset 18
    uint16_t Reserved;       // Offset 20 (moved!)
    uint16_t LastWriteTime;  // Offset 22
    uint16_t LastWriteDate;  // Offset 24
    uint16_t StartCluster;   // Offset 26
    uint32_t FileSize;       // Offset 28
};

uint16_t getFatEntry(uint16_t cluster, struct FileAllocationTable *fat)
{
    uint32_t fatOffset = cluster + (cluster / 2);
    uint16_t value;

    if (cluster % 2 == 0)
    {
        value = fat->Entries[fatOffset] + ((fat->Entries[fatOffset + 1] & 0x0F) << 8);
    }
    else
    {
        value = (fat->Entries[fatOffset] >> 4) + (fat->Entries[fatOffset + 1] << 4);
    }

    return value;
}

int main(int argc, char *argv[])
{
    struct FAT12System fat12;

    // PULL BOOTSECTOR FROM DISK
    struct BootSector bootSector;
    FILE *disk = fopen("disk.img", "rb");
    if (!disk)
    {
        printf("Failed to open disk\n");
        return -1;
    }

    fread(&bootSector, sizeof(struct BootSector), 1, disk);

    fat12.bootsector = bootSector;
    

    // PULL FAT1 FROM DISK
    uint16_t sizePerFat = bootSector.SectorsPerFAT * bootSector.BytesPerSector;
    uint16_t startOffset = bootSector.ReservedSectors * bootSector.BytesPerSector;

    fat12.fat1 = (struct FileAllocationTable *)malloc(sizePerFat);
    fat12.fat2 = (struct FileAllocationTable *)malloc(sizePerFat);

    fseek(disk, startOffset, SEEK_SET);
    
    struct FileAllocationTable *fat = malloc(sizePerFat);
    fread(fat, sizeof(struct FileAllocationTable), 1, disk);

    memcpy(fat12.fat1, fat, sizePerFat);

    // PULL FAT2 FROM DISK

    startOffset = startOffset + sizePerFat;
    fseek(disk, startOffset, SEEK_SET);
    
    struct FileAllocationTable *fat_backup = malloc(sizePerFat);
    fread(fat_backup, sizeof(struct FileAllocationTable), 1, disk);

    memcpy(fat12.fat2, fat_backup, sizePerFat);


    // PULL ROOT DIRECTORY FROM DISK

    startOffset = startOffset + sizePerFat;

    fseek(disk, startOffset, SEEK_SET);

    uint16_t numRootEntries = bootSector.RootEntries;
    uint32_t rootDirSize = numRootEntries * sizeof(struct Directory);

    struct Directory *rootDir = (struct Directory *)malloc(rootDirSize);

    fat12.rootdir = (struct Directory *)malloc(rootDirSize);

    if (!fread(rootDir, rootDirSize, 1, disk))
    {
        printf("Failed to read root directory!\n");
        free(rootDir);
        return -1;
    }

    memcpy(fat12.rootdir, rootDir, rootDirSize);

    // PULL DATA REGION FROM DATA REGION

    uint16_t dataRegionOffset = startOffset + (numRootEntries * 32);

    for (int i = 0; i < numRootEntries; i++)
    {
        if (fat12.rootdir[i].File_Name[0] == 0x00 || fat12.rootdir[i].File_Name[0] == 0xE5)
        {
            continue;
        }

        printf("\nDirectory Entry %d:\n", i);
        printf("Filename: %.8s.%.3s\n", fat12.rootdir[i].File_Name, fat12.rootdir[i].Extension);
        printf("File Size: %u bytes\n", fat12.rootdir[i].FileSize);
        printf("Contents:\n");

        uint16_t currentCluster = fat12.rootdir[i].StartCluster;
        struct DataRegion dataRegion;

        while (currentCluster < 0xFF8)
        {
            uint32_t clusterOffset = dataRegionOffset + ((currentCluster - 2) * 512);

            fseek(disk, clusterOffset, SEEK_SET);

            if (!fread(&dataRegion, sizeof(struct DataRegion), 1, disk))
            {
                printf("Failed to read cluster!\n");
                break;
            }

            for (int j = 0; j < 512 && j < fat12.rootdir[i].FileSize; j++)
            {
                if (dataRegion.Clusters[j] >= 32 && dataRegion.Clusters[j] <= 126)
                {
                    printf("%c", dataRegion.Clusters[j]);
                }
            }
            currentCluster = getFatEntry(currentCluster, fat);
        }
        printf("\n");
    }

    free(fat);
    free(fat_backup);
    free(rootDir);
    free(fat12.fat1);
    free(fat12.fat2);
    free(fat12.rootdir);

    return 0;
}