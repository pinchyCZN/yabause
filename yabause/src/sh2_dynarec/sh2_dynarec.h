#ifndef SH2_DYNAREC_H
#define SH2_DYNAREC_H

void sh2_dynarec_init(void);
int verify_dirty(pointer addr);
void invalidate_all_pages(void);

void YabauseDynarecOneFrameExec(int, int);

#define PROT_READ       0x01            /* page can be read */
#define PROT_WRITE      0x02            /* page can be written */
#define PROT_EXEC       0x04            /* page can be executed */

#define MAP_SHARED      0x001           /* Share changes */
#define MAP_PRIVATE     0x002           /* Changes are private */
#define MAP_TYPE        0x00f           /* Mask for type of mapping */
#define MAP_FIXED       0x010           /* Interpret addr exactly */
#define MAP_ANONYMOUS   0x0800          /* don't use a file */

#define SH2CORE_DYNAREC 2

#endif
