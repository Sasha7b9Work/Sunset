#include <libssh/libssh.h>


#pragma warning(push, 0)


void ssh_clean_pubkey_hash(unsigned char ** /*hash*/)
{

}


char *ssh_get_hexa(const unsigned char * /*what*/, size_t /*len*/)
{
    return nullptr;
}


const char *ssh_get_error(void *error)
{
    return nullptr;
}


int ssh_options_set(ssh_session session, enum ssh_options_e type, const void *value)
{
    return 0;
}


int ssh_get_server_publickey(ssh_session session, ssh_key *key)
{
    return 0;
}


void ssh_key_free(ssh_key key)
{
}


int ssh_get_publickey_hash(const ssh_key key,
    enum ssh_publickey_hash_type type,
    unsigned char **hash,
    size_t *hlen)
{
    return 0;
}


void ssh_free(ssh_session session)
{

}


void ssh_string_free_char(char *s)
{

}


int ssh_session_update_known_hosts(ssh_session session)
{
    return 0;
}


int ssh_userauth_password(ssh_session session,
    const char *username,
    const char *password)
{
    return 0;
}


enum ssh_known_hosts_e ssh_session_is_known_server(ssh_session session)
{
    return SSH_KNOWN_HOSTS_ERROR;
}


ssh_session ssh_new(void)
{
    return nullptr;
}


int ssh_connect(ssh_session session)
{
    return 0;
}


void ssh_disconnect(ssh_session session)
{

}

#pragma warning(pop)
