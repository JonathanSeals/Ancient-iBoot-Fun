/* HFS Plus extent descriptor */
struct HFSPlusExtentDescriptor {
    u_int32_t     startBlock;        /* first allocation block */
    u_int32_t     blockCount;        /* number of allocation blocks */
};
typedef struct HFSPlusExtentDescriptor HFSPlusExtentDescriptor;

/* HFS extent record */
typedef HFSPlusExtentDescriptor HFSExtentRecord[3];

/* HFS Plus extent record */
typedef HFSPlusExtentDescriptor HFSPlusExtentRecord[8];

/* HFS Plus Fork data info - 80 bytes */
struct HFSPlusForkData {
    u_int64_t         logicalSize;    /* fork's logical size in bytes */
    u_int32_t         clumpSize;    /* fork's clump size in bytes */
    u_int32_t         totalBlocks;    /* total blocks used by this fork */
    HFSPlusExtentRecord     extents;    /* initial set of extents */
};
typedef struct HFSPlusForkData HFSPlusForkData;

/* HFS Plus Volume Header - 512 bytes */
/* Stored at sector #2 (3rd sector) and second-to-last sector. */
struct HFSPlusVolumeHeader {
    u_int16_t     signature;        /* == kHFSPlusSigWord */
    u_int16_t     version;        /* == kHFSPlusVersion */
    u_int32_t     attributes;        /* volume attributes */
    u_int32_t     lastMountedVersion;    /* implementation version which last mounted volume */
    u_int32_t     reserved;        /* reserved - initialized as zero */
    
    u_int32_t     createDate;        /* date and time of volume creation */
    u_int32_t     modifyDate;        /* date and time of last modification */
    u_int32_t     backupDate;        /* date and time of last backup */
    u_int32_t     checkedDate;        /* date and time of last disk check */
    
    u_int32_t     fileCount;        /* number of files in volume */
    u_int32_t     folderCount;        /* number of directories in volume */
    
    u_int32_t     blockSize;        /* size (in bytes) of allocation blocks */
    u_int32_t     totalBlocks;        /* number of allocation blocks in volume (includes this header and VBM*/
    u_int32_t     freeBlocks;        /* number of unused allocation blocks */
    
    u_int32_t     nextAllocation;        /* start of next allocation search */
    u_int32_t     rsrcClumpSize;        /* default resource fork clump size */
    u_int32_t     dataClumpSize;        /* default data fork clump size */
    u_int32_t     nextCatalogID;        /* next unused catalog node ID */
    
    u_int32_t     writeCount;        /* volume write count */
    u_int64_t     encodingsBitmap;    /* which encodings have been use  on this volume */
    
    u_int8_t     finderInfo[32];        /* information used by the Finder */
    
    HFSPlusForkData     allocationFile;    /* allocation bitmap file */
    HFSPlusForkData  extentsFile;        /* extents B-tree file */
    HFSPlusForkData  catalogFile;        /* catalog B-tree file */
    HFSPlusForkData  attributesFile;    /* extended attributes B-tree file */
    HFSPlusForkData     startupFile;        /* boot file (secondary loader) */
};
typedef struct HFSPlusVolumeHeader HFSPlusVolumeHeader;

