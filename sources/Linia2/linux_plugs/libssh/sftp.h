#pragma once

#pragma warning(push, 0)
#pragma warning(disable : 4668)

#include <sys/types.h>
 
#include "libssh.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
#ifdef _WIN32
#ifndef uid_t
  typedef uint32_t uid_t;
#endif /* uid_t */
#ifndef gid_t
  typedef uint32_t gid_t;
#endif /* gid_t */
#ifdef _MSC_VER
 
# ifndef _SSIZE_T_DEFINED
#  undef ssize_t
#  include <BaseTsd.h>
   typedef _W64 SSIZE_T ssize_t;
#  define _SSIZE_T_DEFINED
# endif /* _SSIZE_T_DEFINED */
 
#endif /* _MSC_VER */
#endif /* _WIN32 */
 
#define LIBSFTP_VERSION 3
 
typedef struct sftp_attributes_struct* sftp_attributes;
typedef struct sftp_client_message_struct* sftp_client_message;
typedef struct sftp_dir_struct* sftp_dir;
typedef struct sftp_ext_struct *sftp_ext;
typedef struct sftp_file_struct* sftp_file;
typedef struct sftp_message_struct* sftp_message;
typedef struct sftp_packet_struct* sftp_packet;
typedef struct sftp_request_queue_struct* sftp_request_queue;
typedef struct sftp_session_struct* sftp_session;
typedef struct sftp_status_message_struct* sftp_status_message;
typedef struct sftp_statvfs_struct* sftp_statvfs_t;
typedef struct sftp_limits_struct* sftp_limits_t;
typedef struct sftp_aio_struct* sftp_aio;
typedef struct sftp_name_id_map_struct *sftp_name_id_map;
 
struct sftp_session_struct {
    ssh_session session;
    ssh_channel channel;
    int server_version;
    int client_version;
    int version;
    sftp_request_queue queue;
    uint32_t id_counter;
    int errnum;
    void **handles;
    sftp_ext ext;
    sftp_packet read_packet;
    sftp_limits_t limits;
};
 
struct sftp_packet_struct {
    sftp_session sftp;
    uint8_t type;
    ssh_buffer payload;
};
 
/* file handler */
struct sftp_file_struct {
    sftp_session sftp;
    char *name;
    uint64_t offset;
    ssh_string handle;
    int eof;
    int nonblocking;
};
 
struct sftp_dir_struct {
    sftp_session sftp;
    char *name;
    ssh_string handle; /* handle to directory */
    ssh_buffer buffer; /* contains raw attributes from server which haven't been parsed */
    uint32_t count; /* counts the number of following attributes structures into buffer */
    int eof; /* end of directory listing */
};
 
struct sftp_message_struct {
    sftp_session sftp;
    uint8_t packet_type;
    ssh_buffer payload;
    uint32_t id;
};
 
/* this is a bunch of all data that could be into a message */
struct sftp_client_message_struct {
    sftp_session sftp;
    uint8_t type;
    uint32_t id;
    char *filename; /* can be "path" */
    uint32_t flags;
    sftp_attributes attr;
    ssh_string handle;
    uint64_t offset;
    uint32_t len;
    int attr_num;
    ssh_buffer attrbuf; /* used by sftp_reply_attrs */
    ssh_string data; /* can be newpath of rename() */
    ssh_buffer complete_message; /* complete message in case of retransmission*/
    char *str_data; /* cstring version of data */
    char *submessage; /* for extended messages */
};
 
struct sftp_request_queue_struct {
    sftp_request_queue next;
    sftp_message message;
};
 
/* SSH_FXP_MESSAGE described into .7 page 26 */
struct sftp_status_message_struct {
    uint32_t id;
    uint32_t status;
    ssh_string error_unused; /* not used anymore */
    ssh_string lang_unused;  /* not used anymore */
    char *errormsg;
    char *langmsg;
};
 
struct sftp_attributes_struct {
    char *name;
    char *longname; /* ls -l output on openssh, not reliable else */
    uint32_t flags;
    uint8_t type;
    uint64_t size;
    uint32_t uid;
    uint32_t gid;
    char *owner; /* set if openssh and version 4 */
    char *group; /* set if openssh and version 4 */
    uint32_t permissions;
    uint64_t atime64;
    uint32_t atime;
    uint32_t atime_nseconds;
    uint64_t createtime;
    uint32_t createtime_nseconds;
    uint64_t mtime64;
    uint32_t mtime;
    uint32_t mtime_nseconds;
    ssh_string acl;
    uint32_t extended_count;
    ssh_string extended_type;
    ssh_string extended_data;
};
struct sftp_statvfs_struct {
  uint64_t f_bsize;   
  uint64_t f_frsize;  
  uint64_t f_blocks;  
  uint64_t f_bfree;   
  uint64_t f_bavail;  
  uint64_t f_files;   
  uint64_t f_ffree;   
  uint64_t f_favail;  
  uint64_t f_fsid;    
  uint64_t f_flag;    
  uint64_t f_namemax; 
};
struct sftp_limits_struct {
    uint64_t max_packet_length;   
    uint64_t max_read_length;     
    uint64_t max_write_length;    
    uint64_t max_open_handles;    
};
struct sftp_name_id_map_struct {
    uint32_t count;
    uint32_t *ids;
    char **names;
};
LIBSSH_API sftp_session sftp_new(ssh_session session);
LIBSSH_API sftp_session sftp_new_channel(ssh_session session, ssh_channel channel);
 
LIBSSH_API void sftp_free(sftp_session sftp);
LIBSSH_API int sftp_init(sftp_session sftp);
LIBSSH_API int sftp_get_error(sftp_session sftp);
LIBSSH_API unsigned int sftp_extensions_get_count(sftp_session sftp);
LIBSSH_API const char *sftp_extensions_get_name(sftp_session sftp, unsigned int indexn);
LIBSSH_API const char *sftp_extensions_get_data(sftp_session sftp, unsigned int indexn);
LIBSSH_API int sftp_extension_supported(sftp_session sftp, const char *name,
    const char *data);
LIBSSH_API sftp_dir sftp_opendir(sftp_session session, const char *path);
LIBSSH_API sftp_attributes sftp_readdir(sftp_session session, sftp_dir dir);
LIBSSH_API int sftp_dir_eof(sftp_dir dir);
LIBSSH_API sftp_attributes sftp_stat(sftp_session session, const char *path);
LIBSSH_API sftp_attributes sftp_lstat(sftp_session session, const char *path);
LIBSSH_API sftp_attributes sftp_fstat(sftp_file file);
LIBSSH_API void sftp_attributes_free(sftp_attributes file);
LIBSSH_API int sftp_closedir(sftp_dir dir);
LIBSSH_API int sftp_close(sftp_file file);
LIBSSH_API sftp_file sftp_open(sftp_session session, const char *file, int accesstype,
    mode_t mode);
LIBSSH_API void sftp_file_set_nonblocking(sftp_file handle);
LIBSSH_API void sftp_file_set_blocking(sftp_file handle);
LIBSSH_API ssize_t sftp_read(sftp_file file, void *buf, size_t count);
SSH_DEPRECATED LIBSSH_API int sftp_async_read_begin(sftp_file file,
                                                    uint32_t len);
SSH_DEPRECATED LIBSSH_API int sftp_async_read(sftp_file file,
                                              void *data,
                                              uint32_t len,
                                              uint32_t id);
LIBSSH_API ssize_t sftp_write(sftp_file file, const void *buf, size_t count);
LIBSSH_API void sftp_aio_free(sftp_aio aio);
#define SFTP_AIO_FREE(x) \
    do { if(x != NULL) {sftp_aio_free(x); x = NULL;} } while(0)
LIBSSH_API ssize_t sftp_aio_begin_read(sftp_file file,
                                       size_t len,
                                       sftp_aio *aio);
LIBSSH_API ssize_t sftp_aio_wait_read(sftp_aio *aio,
                                      void *buf,
                                      size_t buf_size);
LIBSSH_API ssize_t sftp_aio_begin_write(sftp_file file,
                                        const void *buf,
                                        size_t len,
                                        sftp_aio *aio);
LIBSSH_API ssize_t sftp_aio_wait_write(sftp_aio *aio);
LIBSSH_API int sftp_seek(sftp_file file, uint32_t new_offset);
LIBSSH_API int sftp_seek64(sftp_file file, uint64_t new_offset);
LIBSSH_API unsigned long sftp_tell(sftp_file file);
LIBSSH_API uint64_t sftp_tell64(sftp_file file);
LIBSSH_API void sftp_rewind(sftp_file file);
LIBSSH_API int sftp_unlink(sftp_session sftp, const char *file);
LIBSSH_API int sftp_rmdir(sftp_session sftp, const char *directory);
LIBSSH_API int sftp_mkdir(sftp_session sftp, const char *directory, mode_t mode);
LIBSSH_API int sftp_rename(sftp_session sftp, const char *original, const  char *newname);
LIBSSH_API int sftp_setstat(sftp_session sftp, const char *file, sftp_attributes attr);
LIBSSH_API int
sftp_lsetstat(sftp_session sftp, const char *file, sftp_attributes attr);
LIBSSH_API int sftp_chown(sftp_session sftp, const char *file, uid_t owner, gid_t group);
LIBSSH_API int sftp_chmod(sftp_session sftp, const char *file, mode_t mode);
LIBSSH_API int sftp_utimes(sftp_session sftp, const char *file, const struct timeval *times);
LIBSSH_API int sftp_symlink(sftp_session sftp, const char *target, const char *dest);
LIBSSH_API char *sftp_readlink(sftp_session sftp, const char *path);
LIBSSH_API int sftp_hardlink(sftp_session sftp, const char *oldpath, const char *newpath);
LIBSSH_API sftp_statvfs_t sftp_statvfs(sftp_session sftp, const char *path);
LIBSSH_API sftp_statvfs_t sftp_fstatvfs(sftp_file file);
LIBSSH_API void sftp_statvfs_free(sftp_statvfs_t statvfs_o);
LIBSSH_API int sftp_fsync(sftp_file file);
LIBSSH_API sftp_limits_t sftp_limits(sftp_session sftp);
LIBSSH_API void sftp_limits_free(sftp_limits_t limits);
LIBSSH_API char *sftp_canonicalize_path(sftp_session sftp, const char *path);
LIBSSH_API int sftp_server_version(sftp_session sftp);
LIBSSH_API char *sftp_expand_path(sftp_session sftp, const char *path);
LIBSSH_API char *sftp_home_directory(sftp_session sftp, const char *username);
LIBSSH_API sftp_name_id_map sftp_name_id_map_new(uint32_t count);
LIBSSH_API void sftp_name_id_map_free(sftp_name_id_map map);
LIBSSH_API int sftp_get_users_groups_by_id(sftp_session sftp,
                                           sftp_name_id_map users_map,
                                           sftp_name_id_map groups_map);
 
#ifdef WITH_SERVER
LIBSSH_API sftp_session sftp_server_new(ssh_session session, ssh_channel chan);
SSH_DEPRECATED LIBSSH_API int sftp_server_init(sftp_session sftp);
LIBSSH_API void sftp_server_free(sftp_session sftp);
#endif  /* WITH_SERVER */
 
/* sftpserver.c */
 
LIBSSH_API sftp_client_message sftp_get_client_message(sftp_session sftp);
LIBSSH_API void sftp_client_message_free(sftp_client_message msg);
LIBSSH_API uint8_t sftp_client_message_get_type(sftp_client_message msg);
LIBSSH_API const char *sftp_client_message_get_filename(sftp_client_message msg);
LIBSSH_API void sftp_client_message_set_filename(sftp_client_message msg, const char *newname);
LIBSSH_API const char *sftp_client_message_get_data(sftp_client_message msg);
LIBSSH_API uint32_t sftp_client_message_get_flags(sftp_client_message msg);
LIBSSH_API const char *sftp_client_message_get_submessage(sftp_client_message msg);
LIBSSH_API int sftp_send_client_message(sftp_session sftp, sftp_client_message msg);
LIBSSH_API int sftp_reply_name(sftp_client_message msg, const char *name,
    sftp_attributes attr);
LIBSSH_API int sftp_reply_handle(sftp_client_message msg, ssh_string handle);
LIBSSH_API ssh_string sftp_handle_alloc(sftp_session sftp, void *info);
LIBSSH_API int sftp_reply_attr(sftp_client_message msg, sftp_attributes attr);
LIBSSH_API void *sftp_handle(sftp_session sftp, ssh_string handle);
LIBSSH_API int sftp_reply_status(sftp_client_message msg, uint32_t status, const char *message);
LIBSSH_API int sftp_reply_names_add(sftp_client_message msg, const char *file,
    const char *longname, sftp_attributes attr);
LIBSSH_API int sftp_reply_names(sftp_client_message msg);
LIBSSH_API int sftp_reply_data(sftp_client_message msg, const void *data, int len);
LIBSSH_API void sftp_handle_remove(sftp_session sftp, void *handle);
 
/* SFTP commands and constants */
#define SSH_FXP_INIT 1
#define SSH_FXP_VERSION 2
#define SSH_FXP_OPEN 3
#define SSH_FXP_CLOSE 4
#define SSH_FXP_READ 5
#define SSH_FXP_WRITE 6
#define SSH_FXP_LSTAT 7
#define SSH_FXP_FSTAT 8
#define SSH_FXP_SETSTAT 9
#define SSH_FXP_FSETSTAT 10
#define SSH_FXP_OPENDIR 11
#define SSH_FXP_READDIR 12
#define SSH_FXP_REMOVE 13
#define SSH_FXP_MKDIR 14
#define SSH_FXP_RMDIR 15
#define SSH_FXP_REALPATH 16
#define SSH_FXP_STAT 17
#define SSH_FXP_RENAME 18
#define SSH_FXP_READLINK 19
#define SSH_FXP_SYMLINK 20
 
#define SSH_FXP_STATUS 101
#define SSH_FXP_HANDLE 102
#define SSH_FXP_DATA 103
#define SSH_FXP_NAME 104
#define SSH_FXP_ATTRS 105
 
#define SSH_FXP_EXTENDED 200
#define SSH_FXP_EXTENDED_REPLY 201
 
/* attributes */
/* sftp draft is completely braindead : version 3 and 4 have different flags for same constants */
/* and even worst, version 4 has same flag for 2 different constants */
/* follow up : i won't develop any sftp4 compliant library before having a clarification */
 
#define SSH_FILEXFER_ATTR_SIZE 0x00000001
#define SSH_FILEXFER_ATTR_PERMISSIONS 0x00000004
#define SSH_FILEXFER_ATTR_ACCESSTIME 0x00000008
#define SSH_FILEXFER_ATTR_ACMODTIME  0x00000008
#define SSH_FILEXFER_ATTR_CREATETIME 0x00000010
#define SSH_FILEXFER_ATTR_MODIFYTIME 0x00000020
#define SSH_FILEXFER_ATTR_ACL 0x00000040
#define SSH_FILEXFER_ATTR_OWNERGROUP 0x00000080
#define SSH_FILEXFER_ATTR_SUBSECOND_TIMES 0x00000100
#define SSH_FILEXFER_ATTR_EXTENDED 0x80000000
#define SSH_FILEXFER_ATTR_UIDGID 0x00000002
 
/* types */
#define SSH_FILEXFER_TYPE_REGULAR 1
#define SSH_FILEXFER_TYPE_DIRECTORY 2
#define SSH_FILEXFER_TYPE_SYMLINK 3
#define SSH_FILEXFER_TYPE_SPECIAL 4
#define SSH_FILEXFER_TYPE_UNKNOWN 5
#define SSH_FX_OK 0
#define SSH_FX_EOF 1
#define SSH_FX_NO_SUCH_FILE 2
#define SSH_FX_PERMISSION_DENIED 3
#define SSH_FX_FAILURE 4
#define SSH_FX_BAD_MESSAGE 5
#define SSH_FX_NO_CONNECTION 6
#define SSH_FX_CONNECTION_LOST 7
#define SSH_FX_OP_UNSUPPORTED 8
#define SSH_FX_INVALID_HANDLE 9
#define SSH_FX_NO_SUCH_PATH 10
#define SSH_FX_FILE_ALREADY_EXISTS 11
#define SSH_FX_WRITE_PROTECT 12
#define SSH_FX_NO_MEDIA 13
 
/* file flags */
#define SSH_FXF_READ 0x01
#define SSH_FXF_WRITE 0x02
#define SSH_FXF_APPEND 0x04
#define SSH_FXF_CREAT 0x08
#define SSH_FXF_TRUNC 0x10
#define SSH_FXF_EXCL 0x20
#define SSH_FXF_TEXT 0x40
 
/* file type flags */
#define SSH_S_IFMT   00170000
#define SSH_S_IFSOCK 0140000
#define SSH_S_IFLNK  0120000
#define SSH_S_IFREG  0100000
#define SSH_S_IFBLK  0060000
#define SSH_S_IFDIR  0040000
#define SSH_S_IFCHR  0020000
#define SSH_S_IFIFO  0010000
 
/* rename flags */
#define SSH_FXF_RENAME_OVERWRITE  0x00000001
#define SSH_FXF_RENAME_ATOMIC     0x00000002
#define SSH_FXF_RENAME_NATIVE     0x00000004
 
#define SFTP_OPEN SSH_FXP_OPEN
#define SFTP_CLOSE SSH_FXP_CLOSE
#define SFTP_READ SSH_FXP_READ
#define SFTP_WRITE SSH_FXP_WRITE
#define SFTP_LSTAT SSH_FXP_LSTAT
#define SFTP_FSTAT SSH_FXP_FSTAT
#define SFTP_SETSTAT SSH_FXP_SETSTAT
#define SFTP_FSETSTAT SSH_FXP_FSETSTAT
#define SFTP_OPENDIR SSH_FXP_OPENDIR
#define SFTP_READDIR SSH_FXP_READDIR
#define SFTP_REMOVE SSH_FXP_REMOVE
#define SFTP_MKDIR SSH_FXP_MKDIR
#define SFTP_RMDIR SSH_FXP_RMDIR
#define SFTP_REALPATH SSH_FXP_REALPATH
#define SFTP_STAT SSH_FXP_STAT
#define SFTP_RENAME SSH_FXP_RENAME
#define SFTP_READLINK SSH_FXP_READLINK
#define SFTP_SYMLINK SSH_FXP_SYMLINK
#define SFTP_EXTENDED SSH_FXP_EXTENDED
 
/* openssh flags */
#define SSH_FXE_STATVFS_ST_RDONLY 0x1 /* read-only */
#define SSH_FXE_STATVFS_ST_NOSUID 0x2 /* no setuid */
 
#ifdef __cplusplus
}
#endif
 
#pragma warning(pop)
