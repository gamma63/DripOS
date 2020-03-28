#ifndef VFS_H
#define VFS_H
#include <stdint.h>
#include "fs/filesystems/filesystems.h"

struct vfs_node;
typedef struct vfs_node vfs_node_t;

typedef struct fd_entry {
    vfs_node_t *node;
    uint64_t seek;
    int mode;
} fd_entry_t;

/* VFS op types */
typedef int (*vfs_open_t)(char *, int);
typedef int (*vfs_close_t)(fd_entry_t *);;
typedef int (*vfs_read_t)(fd_entry_t *, void *, uint64_t);
typedef int (*vfs_write_t)(fd_entry_t *, void *, uint64_t);
typedef int (*vfs_seek_t)(fd_entry_t *, uint64_t, int);

typedef struct {
    vfs_open_t open;
    vfs_close_t close;
    vfs_read_t read;
    vfs_write_t write;
    vfs_seek_t seek;
} vfs_ops_t;

typedef struct vfs_node {
    char *name;
    vfs_ops_t ops;
    struct vfs_node *parent; // Parent
    struct vfs_node **children; // An array of children
    struct vfs_node *mountpoint; // Mountpoint node
    
    uint64_t children_array_size;

    uint64_t unid; // Unique node id
} vfs_node_t;

void vfs_init();
vfs_node_t *vfs_new_node(char *name, vfs_ops_t ops);
void vfs_add_child(vfs_node_t *parent, vfs_node_t *child);
void create_missing_nodes_from_path(char *path, vfs_ops_t ops, vfs_node_t *mountpoint);
vfs_node_t *get_node_from_path(char *path);
char *get_full_path(vfs_node_t *node);

/* VFS ops */
vfs_node_t *vfs_open(char *name, int mode);
int vfs_close(fd_entry_t *node);
int vfs_read(fd_entry_t *node, void *buf, uint64_t count);
int vfs_write(fd_entry_t *node, void *buf, uint64_t count);
int vfs_seek(fd_entry_t *node, uint64_t offset, int whence);

extern vfs_node_t *root_node;
extern vfs_ops_t dummy_ops;

#endif