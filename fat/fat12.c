#include <stdlib.h>
#include <stdio.h>

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

uint16_t swap16(uint16_t val)
{
    return (val << 8) | (val >> 8);
}

uint16_t getFatEntry(uint16_t cluster, struct FileAllocationTable *fat)
{
    uint32_t fatOffset = cluster + (cluster / 2); // multiply by 1.5
    uint16_t value;

    if (cluster % 2 == 0)
    {
        // Even: take 12 bits from current position
        value = fat->Entries[fatOffset] + ((fat->Entries[fatOffset + 1] & 0x0F) << 8);
    }
    else
    {
        // Odd: take 12 bits shifted
        value = (fat->Entries[fatOffset] >> 4) + (fat->Entries[fatOffset + 1] << 4);
    }

    return value;
}

int main(int argc, char *argv[])
{

    struct BootSector bootSector;
    FILE *disk = fopen("disk.img", "rb");
    if (!disk)
    {
        printf("Failed to open disk\n");
        return -1;
    }

    fread(&bootSector, sizeof(struct BootSector), 1, disk);

    if (0)
    {
        printf("Size of BootSector Struct: %lu\n", sizeof(struct BootSector));
        printf("OEM Identifier: %.8s\n", bootSector.OEM_Identifier);
        printf("Bytes Per Sector: %u\n", bootSector.BytesPerSector);
        printf("Sectors Per Cluster: %u\n", bootSector.SectorsPerCluster);
        printf("Reserved Sectors: %u\n", bootSector.ReservedSectors);
        printf("Number of FATs: %u\n", bootSector.NumberOfFATs);
        printf("Root Entries: %u\n", bootSector.RootEntries);
        printf("Number of Sectors: %u\n", bootSector.TotalSectors);
        printf("Media Descriptor: 0x%02X\n", bootSector.MediaDescriptor);
        printf("Sectors Per FAT: %u\n", bootSector.SectorsPerFAT);
        printf("Sectors Per Head: %u\n", bootSector.SectorsPerTrack);
        printf("Hidden Sectors: %u\n", bootSector.HiddenSectors);
        printf("Drive Number: %u\n", bootSector.DriveNumber);
        printf("Volume ID: 0x%08X\n", bootSector.VolumeID);
        printf("File System Type: %.8s\n", bootSector.FileSystemType);
    }

    uint16_t sizePerFat = bootSector.SectorsPerFAT * bootSector.BytesPerSector;
    uint16_t startOffset = bootSector.ReservedSectors * bootSector.BytesPerSector;

    if (fseek(disk, startOffset, SEEK_SET))
    {
        printf("Fseek failed!\n");
        return -1;
    }

    struct FileAllocationTable fat;
    if (!fread(&fat, sizeof(struct FileAllocationTable), 1, disk))
    {
        printf("Failed to read disk!\n");
        return -1;
    }

    startOffset = startOffset + sizePerFat;

    if (fseek(disk, startOffset, SEEK_SET))
    {
        printf("Fseek failed!\n");
        return -1;
    }

    struct FileAllocationTable fat_backup;
    if (!fread(&fat_backup, sizeof(struct FileAllocationTable), 1, disk))
    {
        printf("Failed to read disk!\n");
        return -1;
    }

    startOffset = startOffset + sizePerFat;

    fseek(disk, startOffset, SEEK_SET);

    uint16_t numRootEntries = bootSector.RootEntries;
    uint32_t rootDirSize = numRootEntries * sizeof(struct Directory);
    struct Directory *rootDir = (struct Directory *)malloc(rootDirSize);

    if (!fread(rootDir, rootDirSize, 1, disk))
    {
        printf("Failed to read root directory!\n");
        free(rootDir);
        return -1;
    }

    uint16_t dataRegionOffset = startOffset + (numRootEntries * 32);
    for (int i = 0; i < numRootEntries; i++)
    {
        if (rootDir[i].File_Name[0] == 0x00 || rootDir[i].File_Name[0] == 0xE5)
        {
            continue;
        }

        printf("\nDirectory Entry %d:\n", i);
        printf("Filename: %.8s.%.3s\n", rootDir[i].File_Name, rootDir[i].Extension);
        // printf("Start Cluster: %u\n", rootDir[i].StartCluster);
        // printf("Physical Cluster: %u\n", getFatEntry(rootDir[i].StartCluster, &fat));
        printf("File Size: %u bytes\n", rootDir[i].FileSize);
        printf("Contents:\n");

        uint16_t currentCluster = rootDir[i].StartCluster;
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

            for (int j = 0; j < 512 && j < rootDir[i].FileSize; j++)
            {
                if (dataRegion.Clusters[j] >= 32 && dataRegion.Clusters[j] <= 126)
                {
                    printf("%c", dataRegion.Clusters[j]);
                }
            }
            currentCluster = getFatEntry(currentCluster, &fat);
        }
        printf("\n");
    }

    free(rootDir);

    return 0;
}