#ifdef __VMA_H__
#else
#define __VMA_H__

struct vma {
  struct file* file;
  char used;
  uint64 ustart;
  uint64 uend; // [ustart, uend)
  uint64 ustart_orig;
  uint64 uend_orig;
  uint length;
  int prot;
  int flags;
  uint offset;
};

#endif
