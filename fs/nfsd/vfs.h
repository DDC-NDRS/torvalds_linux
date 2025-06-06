/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 1995-1997 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_NFSD_VFS_H
#define LINUX_NFSD_VFS_H

#include <linux/fs.h>
#include <linux/posix_acl.h>
#include "nfsfh.h"
#include "nfsd.h"

/*
 * Flags for nfsd_permission
 */
#define NFSD_MAY_NOP			0
#define NFSD_MAY_EXEC			0x001 /* == MAY_EXEC */
#define NFSD_MAY_WRITE			0x002 /* == MAY_WRITE */
#define NFSD_MAY_READ			0x004 /* == MAY_READ */
#define NFSD_MAY_SATTR			0x008
#define NFSD_MAY_TRUNC			0x010
#define NFSD_MAY_NLM			0x020 /* request is from lockd */
#define NFSD_MAY_MASK			0x03f

/* extra hints to permission and open routines: */
#define NFSD_MAY_OWNER_OVERRIDE		0x040
#define NFSD_MAY_LOCAL_ACCESS		0x080 /* for device special files */
#define NFSD_MAY_BYPASS_GSS_ON_ROOT	0x100
#define NFSD_MAY_NOT_BREAK_LEASE	0x200
#define NFSD_MAY_BYPASS_GSS		0x400
#define NFSD_MAY_READ_IF_EXEC		0x800

#define NFSD_MAY_64BIT_COOKIE		0x1000 /* 64 bit readdir cookies for >= NFSv3 */

#define NFSD_MAY_LOCALIO		0x2000 /* for tracing, reflects when localio used */

#define NFSD_MAY_CREATE		(NFSD_MAY_EXEC|NFSD_MAY_WRITE)
#define NFSD_MAY_REMOVE		(NFSD_MAY_EXEC|NFSD_MAY_WRITE|NFSD_MAY_TRUNC)

struct nfsd_file;

/*
 * Callback function for readdir
 */
typedef int (*nfsd_filldir_t)(void *, const char *, int, loff_t, u64, unsigned);

/* nfsd/vfs.c */
struct nfsd_attrs {
	struct iattr		*na_iattr;	/* input */
	struct xdr_netobj	*na_seclabel;	/* input */
	struct posix_acl	*na_pacl;	/* input */
	struct posix_acl	*na_dpacl;	/* input */

	int			na_labelerr;	/* output */
	int			na_aclerr;	/* output */
};

static inline void nfsd_attrs_free(struct nfsd_attrs *attrs)
{
	posix_acl_release(attrs->na_pacl);
	posix_acl_release(attrs->na_dpacl);
}

static inline bool nfsd_attrs_valid(struct nfsd_attrs *attrs)
{
	struct iattr *iap = attrs->na_iattr;

	return (iap->ia_valid || (attrs->na_seclabel &&
		attrs->na_seclabel->len));
}

__be32		nfserrno (int errno);
int		nfsd_cross_mnt(struct svc_rqst *rqstp, struct dentry **dpp,
		                struct svc_export **expp);
__be32		nfsd_lookup(struct svc_rqst *, struct svc_fh *,
				const char *, unsigned int, struct svc_fh *);
__be32		 nfsd_lookup_dentry(struct svc_rqst *, struct svc_fh *,
				const char *, unsigned int,
				struct svc_export **, struct dentry **);
__be32		nfsd_setattr(struct svc_rqst *, struct svc_fh *,
			     struct nfsd_attrs *, const struct timespec64 *);
int nfsd_mountpoint(struct dentry *, struct svc_export *);
#ifdef CONFIG_NFSD_V4
__be32		nfsd4_vfs_fallocate(struct svc_rqst *, struct svc_fh *,
				    struct file *, loff_t, loff_t, int);
__be32		nfsd4_clone_file_range(struct svc_rqst *rqstp,
				       struct nfsd_file *nf_src, u64 src_pos,
				       struct nfsd_file *nf_dst, u64 dst_pos,
				       u64 count, bool sync);
#endif /* CONFIG_NFSD_V4 */
__be32		nfsd_create_locked(struct svc_rqst *, struct svc_fh *,
				struct nfsd_attrs *attrs, int type, dev_t rdev,
				struct svc_fh *res);
__be32		nfsd_create(struct svc_rqst *, struct svc_fh *,
				char *name, int len, struct nfsd_attrs *attrs,
				int type, dev_t rdev, struct svc_fh *res);
__be32		nfsd_access(struct svc_rqst *, struct svc_fh *, u32 *, u32 *);
__be32		nfsd_create_setattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
				struct svc_fh *resfhp, struct nfsd_attrs *iap);
__be32		nfsd_commit(struct svc_rqst *rqst, struct svc_fh *fhp,
				struct nfsd_file *nf, u64 offset, u32 count,
				__be32 *verf);
#ifdef CONFIG_NFSD_V4
__be32		nfsd_getxattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
			    char *name, void **bufp, int *lenp);
__be32		nfsd_listxattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
			    char **bufp, int *lenp);
__be32		nfsd_removexattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
			    char *name);
__be32		nfsd_setxattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
			    char *name, void *buf, u32 len, u32 flags);
#endif
int 		nfsd_open_break_lease(struct inode *, int);
__be32		nfsd_open(struct svc_rqst *, struct svc_fh *, umode_t,
				int, struct file **);
int		nfsd_open_verified(struct svc_fh *fhp, int may_flags,
				struct file **filp);
__be32		nfsd_splice_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
				struct file *file, loff_t offset,
				unsigned long *count,
				u32 *eof);
__be32		nfsd_iter_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
				struct file *file, loff_t offset,
				unsigned long *count, unsigned int base,
				u32 *eof);
bool		nfsd_read_splice_ok(struct svc_rqst *rqstp);
__be32		nfsd_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
				loff_t offset, unsigned long *count,
				u32 *eof);
__be32		nfsd_write(struct svc_rqst *rqstp, struct svc_fh *fhp,
				loff_t offset, const struct xdr_buf *payload,
				unsigned long *cnt, int stable, __be32 *verf);
__be32		nfsd_vfs_write(struct svc_rqst *rqstp, struct svc_fh *fhp,
				struct nfsd_file *nf, loff_t offset,
				const struct xdr_buf *payload,
				unsigned long *cnt, int stable, __be32 *verf);
__be32		nfsd_readlink(struct svc_rqst *, struct svc_fh *,
				char *, int *);
__be32		nfsd_symlink(struct svc_rqst *, struct svc_fh *,
			     char *name, int len, char *path,
			     struct nfsd_attrs *attrs,
			     struct svc_fh *res);
__be32		nfsd_link(struct svc_rqst *, struct svc_fh *,
				char *, int, struct svc_fh *);
ssize_t		nfsd_copy_file_range(struct file *, u64,
				     struct file *, u64, u64);
__be32		nfsd_rename(struct svc_rqst *,
				struct svc_fh *, char *, int,
				struct svc_fh *, char *, int);
__be32		nfsd_unlink(struct svc_rqst *, struct svc_fh *, int type,
				char *name, int len);
__be32		nfsd_readdir(struct svc_rqst *, struct svc_fh *,
			     loff_t *, struct readdir_cd *, nfsd_filldir_t);
__be32		nfsd_statfs(struct svc_rqst *, struct svc_fh *,
				struct kstatfs *, int access);

__be32		nfsd_permission(struct svc_cred *cred, struct svc_export *exp,
				struct dentry *dentry, int acc);

void		nfsd_filp_close(struct file *fp);

static inline int fh_want_write(struct svc_fh *fh)
{
	int ret;

	if (fh->fh_want_write)
		return 0;
	ret = mnt_want_write(fh->fh_export->ex_path.mnt);
	if (!ret)
		fh->fh_want_write = true;
	return ret;
}

static inline void fh_drop_write(struct svc_fh *fh)
{
	if (fh->fh_want_write) {
		fh->fh_want_write = false;
		mnt_drop_write(fh->fh_export->ex_path.mnt);
	}
}

static inline __be32 fh_getattr(const struct svc_fh *fh, struct kstat *stat)
{
	u32 request_mask = STATX_BASIC_STATS;
	struct path p = {.mnt = fh->fh_export->ex_path.mnt,
			 .dentry = fh->fh_dentry};

	if (fh->fh_maxsize == NFS4_FHSIZE)
		request_mask |= (STATX_BTIME | STATX_CHANGE_COOKIE);

	return nfserrno(vfs_getattr(&p, stat, request_mask,
				    AT_STATX_SYNC_AS_STAT));
}

#endif /* LINUX_NFSD_VFS_H */
