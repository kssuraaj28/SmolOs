#ifndef DRIVER_PCI_H
#define DRIVER_PCI_H

#include <stdint.h>

#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

#define BUS_PER_DOMAIN           256
#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      8

#define GEN_ADDR(b,d,f) (0x80000000 | b<<16 | d<<11 | f << 8)

// Configuration space offset for endpoint devices
#define PCI_ENDPT_VENDOR_ID            0x00
#define PCI_ENDPT_DEVICE_ID            0x02
#define PCI_ENDPT_COMMAND              0x04
#define PCI_ENDPT_STATUS               0x06
#define PCI_ENDPT_REVISION_ID          0x08
#define PCI_ENDPT_PROG_IF              0x09
#define PCI_ENDPT_SUBCLASS             0x0a
#define PCI_ENDPT_CLASS                0x0b
#define PCI_ENDPT_CACHE_LINE_SIZE      0x0c
#define PCI_ENDPT_LATENCY_TIMER        0x0d
#define PCI_ENDPT_HEADER_TYPE          0x0e
#define PCI_ENDPT_BIST                 0x0f
#define PCI_ENDPT_BAR0                 0x10
#define PCI_ENDPT_BAR1                 0x14
#define PCI_ENDPT_BAR2                 0x18
#define PCI_ENDPT_BAR3                 0x1C
#define PCI_ENDPT_BAR4                 0x20
#define PCI_ENDPT_BAR5                 0x24
#define PCI_ENDPT_INTERRUPT_LINE       0x3C

#define PCI_BRG_VENDOR_ID            0x00
#define PCI_BRG_DEVICE_ID            0x02
#define PCI_BRG_COMMAND              0x04
#define PCI_BRG_STATUS               0x06
#define PCI_BRG_REVISION_ID          0x08
#define PCI_BRG_PROG_IF              0x09
#define PCI_BRG_SUBCLASS             0x0a
#define PCI_BRG_CLASS                0x0b
#define PCI_BRG_CACHE_LINE_SIZE      0x0c
#define PCI_BRG_LATENCY_TIMER        0x0d
#define PCI_BRG_HEADER_TYPE          0x0e
#define PCI_BRG_BIST                 0x0f
#define PCI_BRG_BAR0                 0x10
#define PCI_BRG_BAR1                 0x14
#define PCI_BRG_PRI_BUS              0x18
#define PCI_BRG_SEC_BUS              0x19
#define PCI_BRG_SUB_BUS              0x1A

#define PCI_HEADER_DEVICE  0
#define PCI_HEADER_BRIDGE  1
#define PCI_HEADER_CARDBUS 2

#define PCI_INV (-1)

typedef struct pci_dev
{
	uint16_t device_id;
	uint16_t vendor_id;

	uint16_t bus;
	uint8_t slot; //device
	uint8_t func;
}pci_dev_t;

#define PCI_READ_BYTE(b,s,f,o) ((pci_config_read(b,s,f,o&0xFC)) >> (8*(o%4)))&0xFF
#define PCI_READ_WORD(b,s,f,o) ((pci_config_read(b,s,f,o&0xFC)) >> (8*(o&2)))&0xFFFF


void pci_scan(pci_dev_t* input);
uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset,uint32_t value);
#endif
