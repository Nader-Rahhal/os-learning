#include <stdlib.h>
#include <stdio.h>

#pragma pack(push, 1)
struct BootSector {
    uint8_t  JumpBoot[3];         // Jump instruction
    uint8_t  OEM_Identifier[8];   // OEM identifier
    uint16_t BytesPerSector;      // Bytes per sector (usually 512)
    uint8_t  SectorsPerCluster;   // Sectors per cluster
    uint16_t ReservedSectors;     // Reserved sectors
    uint8_t  NumberOfFATs;        // Number of FATs
    uint16_t RootEntries;         // Root directory entries
    uint16_t TotalSectors;        // Total sectors (if < 65535)
    uint8_t  MediaDescriptor;     // Media descriptor
    uint16_t SectorsPerFAT;       // Sectors per FAT
    uint16_t SectorsPerTrack;     // Sectors per track
    uint16_t NumberOfHeads;       // Number of heads
    uint32_t HiddenSectors;       // Hidden sectors
    uint32_t TotalSectorsBig;     // Total sectors (if >= 65535)
    uint8_t  DriveNumber;         // Drive number
    uint8_t  Reserved1;           // Reserved
    uint8_t  BootSignature;       // Extended boot signature
    uint32_t VolumeID;           // Volume serial number
    uint8_t  VolumeLabel[11];     // Volume label
    uint8_t  FileSystemType[8];   // File system type
    uint8_t  BootCode[448];       // Boot code
    uint16_t BootSignature2;      // Boot signature (0xAA55)
};
#pragma pack(pop)

struct FileAllocationTable {

};

struct Directory {
    uint8_t File_Name[8]; // Offset 0
    uint8_t Extension[3]; // Offset 8
    uint8_t Attributes; // Offset 11
    uint16_t Reserved; // Offset 12
    uint16_t CreationTime; // Offset 14
    uint16_t CreationDate; // Offset 16
    uint16_t LastAccessDate; // Offset 18
    uint16_t HighCluster; // Offset 20
    uint16_t LastWriteTime; // Offset 22
    uint16_t LastWriteDate; // Offset 24
    uint16_t FirstLogicalCluster; // Offset 26
    uint32_t FileSize; // Offset 28
};

uint16_t swap16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

int main(int argc, char *argv[]){

    struct BootSector bootSector;
    FILE *disk = fopen("disk.img", "rb");
    if (!disk){
        printf("Failed to open disk\n");
        return -1;
    }

    fread(&bootSector, sizeof(struct BootSector), 1, disk);
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
     printf("Volume ID: 0x%08X\n", bootSector.VolumeID);
     printf("File System Type: %.8s\n", bootSector.FileSystemType);
    return 0;
}