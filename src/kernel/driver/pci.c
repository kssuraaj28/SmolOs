#include <stdint.h>
#include <port_io.h>
#include <common.h>
#include <driver/pci.h>
#include <debug/qemu_print.h>
#include <debug/panic.h>
/* Here 'slot' is used for device*/

uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
	if (offset % 4) PANIC ("Unaligned pci offset");

    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
 
    uint32_t address = GEN_ADDR(lbus,lslot,lfunc) + offset ;
 
    outportl(PCI_CONFIG_ADDRESS, address);
    uint32_t tmp = inportl(PCI_CONFIG_DATA);
    return (tmp);
}

void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
{

	if (offset % 4) PANIC ("Unaligned pci offset");
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
 
    uint32_t address = GEN_ADDR(lbus,lslot,lfunc) + offset ;
    outportl(PCI_CONFIG_ADDRESS, address);
    outportl(PCI_CONFIG_DATA, value);
}


void pci_scan(pci_dev_t* result)
{
     for(uint16_t bus = 0; bus < BUS_PER_DOMAIN; bus++) 
		 for(uint8_t device = 0; device < DEVICE_PER_BUS; device++) 
			 for(uint8_t func = 0; func < FUNCTION_PER_DEVICE; func++) 
				 {
					 uint16_t ven = PCI_READ_WORD(bus,device,func,PCI_ENDPT_VENDOR_ID);
					 uint16_t dev = PCI_READ_WORD(bus,device,func,PCI_ENDPT_DEVICE_ID);
					 if (ven == (uint16_t)PCI_INV) continue;
					 if (result -> device_id ==  dev && result -> vendor_id == ven)
					 {
						 result -> bus = bus;
						 result -> slot = device;
						 result -> func = func;
						 return;
					 }
				 }
	 memset(result,0,sizeof(pci_dev_t));
}

void put_pcidev_info(uint16_t bus, uint8_t device, uint8_t func, void (*puts)(char*))
{

	 uint32_t val = pci_config_read(bus,device,func,0);
	 if (val == (uint32_t)PCI_INV) {puts("\nNo device"); return;}

	 puts("\n\nBus: ");puts(int2hex(bus));
	 puts("\tSlot: ");puts(int2hex(device));
	 puts("\tFunc: ");puts(int2hex(func));

	 val = PCI_READ_WORD(bus,device,func,PCI_ENDPT_VENDOR_ID);
	 puts("\nVendor: ");puts(int2hex(val));
	 val = PCI_READ_WORD(bus,device,func,PCI_ENDPT_DEVICE_ID);
	 puts("\tDevice: ");puts(int2hex(val));

	 val = PCI_READ_BYTE(bus,device,func,PCI_ENDPT_CLASS);
	 puts("\nClass: ");puts(int2hex(val));
	 val = PCI_READ_BYTE(bus,device,func,PCI_ENDPT_SUBCLASS);
	 puts("\tSubclass: ");puts(int2hex(val));

	 val = PCI_READ_BYTE(bus,device,func,PCI_ENDPT_HEADER_TYPE);
	 puts("\nHeader: ");puts(int2hex(val));
	 switch (val & 0xF)
	 {
		 case 0x0:
			 puts(" Endpoint");
			 puts("\nBAR0: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR0)));
			 puts("\nBAR1: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR1)));
			 puts("\nBAR2: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR2)));
			 puts("\nBAR3: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR3)));
			 puts("\nBAR4: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR4)));
			 puts("\nBAR5: ");puts(int2hex(pci_config_read(bus,device,func,PCI_ENDPT_BAR5)));
			 break;
		 case 0x1:
			 puts(" PCI2PCI bridge");
			 puts("\nBAR0: ");puts(int2hex(pci_config_read(bus,device,func,PCI_BRG_BAR0)));
			 puts("\nBAR1: ");puts(int2hex(pci_config_read(bus,device,func,PCI_BRG_BAR1)));
			 puts("\nPrimBus: ");puts(int2hex(pci_config_read(bus,device,func,PCI_BRG_PRI_BUS)));
			 puts("\nSecBus: ");puts(int2hex(pci_config_read(bus,device,func,PCI_BRG_SEC_BUS)));
			 puts("\nSubBus: ");puts(int2hex(pci_config_read(bus,device,func,PCI_BRG_SUB_BUS)));
			 break;
		 case 0x2:
			 puts(" Cardbus device");
			 break;
	 }

}
