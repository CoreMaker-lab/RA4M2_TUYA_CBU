/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (1)
#endif
/* ISR prototypes */
void r_icu_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 0) /* ICU IRQ4 (External pin interrupt 4) */
#define ICU_IRQ4_IRQn          ((IRQn_Type) 0) /* ICU IRQ4 (External pin interrupt 4) */
/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (1)

#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
