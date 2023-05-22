CC=x86_64-w64-mingw32-gcc
CFLAGS = -shared -nostdlib -mno-red-zone -fno-stack-protector -Wall -e EfiMain

default:tetris.iso

%.dll: %.c
	$(CC) $(CFLAGS) $< -o $@

%.efi: %.dll
	objcopy --target=efi-app-x86_64 $< $@

tetris.iso: tetris.efi
	mkdir -p ./iso
	mv $< bootx64.efi
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/boot
	mcopy -i fat.img bootx64.efi ::/EFI/boot
	cp fat.img iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o $@ iso

clean:
	- rm *.dll *.efi *.img *.iso
	rm -rf ./iso
	