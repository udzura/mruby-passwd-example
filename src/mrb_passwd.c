/*
** mrb_passwd.c - Passwd class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/class.h>
#include "mrb_passwd.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct mrb_passwd {
  struct passwd *pwd;
  char *buf;
} mrb_passwd;

static void mrb_passwd_free(mrb_state *mrb, void *p)
{
  mrb_passwd *pwd = (mrb_passwd *)p;
  free(pwd->pwd);
  mrb_free(mrb, pwd);
}

static const struct mrb_data_type mrb_passwd_data_type = {
  "mrb_passwd", mrb_passwd_free,
};

static mrb_value mrb_passwd_init(mrb_state *mrb, mrb_value self)
{
  mrb_passwd *data;
  char *name;
  struct passwd pwd;
  size_t bufsize;
  bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
    bufsize = 16384;
  char buf[bufsize];
  struct passwd *res;

  data = (mrb_passwd *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_passwd_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "z", &name);
  data = (mrb_passwd *)mrb_malloc(mrb, sizeof(mrb_passwd));
  data->pwd = malloc(sizeof(struct passwd));

  if(getpwnam_r(name, &pwd, buf, bufsize, &res) < 0)
    mrb_sys_fail(mrb, "getpwnam failed");
  else if(!res) {
    mrb_sys_fail(mrb, "entry not found");
  }

  /* data->pwd = res cannot escape struct from stack...; */
  /* data->pwd->pw_name  = res->pw_name; */
  /* data->pwd->pw_uid   = res->pw_uid; */
  /* data->pwd->pw_gid   = res->pw_gid; */
  /* data->pwd->pw_gecos = res->pw_gecos; */
  /* data->pwd->pw_dir   = res->pw_dir; */
  /* data->pwd->pw_shell = res->pw_shell; */
  memcpy(data->pwd, res, sizeof(struct passwd));

  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_passwd_uid(mrb_state *mrb, mrb_value self)
{
  mrb_passwd *pwd = DATA_PTR(self);
  return mrb_fixnum_value((int)(pwd->pwd->pw_uid));
}

static mrb_value mrb_passwd_shell(mrb_state *mrb, mrb_value self)
{
  mrb_passwd *pwd = DATA_PTR(self);
  return mrb_str_new_cstr(mrb, pwd->pwd->pw_shell);
}

void mrb_mruby_passwd_example_gem_init(mrb_state *mrb)
{
    struct RClass *passwd;
    passwd = mrb_define_class(mrb, "Passwd", mrb->object_class);
    MRB_SET_INSTANCE_TT(passwd, MRB_TT_DATA);
    mrb_define_method(mrb, passwd, "initialize", mrb_passwd_init, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, passwd, "uid", mrb_passwd_uid, MRB_ARGS_NONE());
    mrb_define_method(mrb, passwd, "shell", mrb_passwd_shell, MRB_ARGS_NONE());

    DONE;
}

void mrb_mruby_passwd_example_gem_final(mrb_state *mrb)
{
}
