#include <libssh/sftp.h>


#pragma warning(push, 0)


char *sftp_canonicalize_path(sftp_session /*sftp*/, const char * /*path*/)
{
    return nullptr;
}


int sftp_rename(sftp_session /*sftp*/, const char * /*original*/, const  char * /*newname*/)
{
    return 0;
}


int sftp_unlink(sftp_session /*sftp*/, const char * /*file*/)
{
    return 0;
}


ssize_t sftp_read(sftp_file /*file*/, void * /*buf*/, size_t /*count*/)
{
    return 0;
}


ssize_t sftp_write(sftp_file /*file*/, const void * /*buf*/, size_t /*count*/)
{
    return 0;
}


sftp_file sftp_open(sftp_session /*session*/, const char * /*file*/, int /*accesstype*/, mode_t /*mode*/)
{
    return nullptr;
}


sftp_dir sftp_opendir(sftp_session /*session*/, const char * /*path*/)
{
    return nullptr;
}


void sftp_free(sftp_session /*sftp*/)
{

}


void sftp_attributes_free(sftp_attributes /*file*/)
{

}


int sftp_closedir(sftp_dir dir)
{
    return 0;
}


int sftp_close(sftp_file file)
{
    return 0;
}


sftp_attributes sftp_readdir(sftp_session session, sftp_dir dir)
{
    return nullptr;
}


int sftp_init(sftp_session sftp)
{
    return 0;
}


int sftp_dir_eof(sftp_dir dir)
{
    return 0;
}


int sftp_rmdir(sftp_session sftp, const char *directory)
{
    return 0;
}


sftp_attributes sftp_stat(sftp_session session, const char *path)
{
    return nullptr;
}


sftp_session sftp_new(ssh_session session)
{
    return nullptr;
}


int sftp_mkdir(sftp_session sftp, const char *directory, mode_t mode)
{
    return 0;
}

#pragma warning(pop)
