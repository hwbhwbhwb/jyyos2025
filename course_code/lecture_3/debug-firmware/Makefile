minimal.img: Makefile
	( echo "eb fe 11 22 33 44" | xxd -r -p; \
	  cat /dev/zero | head -c 504; \
	  echo "55 aa" | xxd -r -p \
	) > minimal.img	

# Create a minimal bootable image（生成磁盘镜像文件）
# hwb@hwb-virtual-machine:~/jyyos/os2025/course_code/lecture_3/debug-firmware$ file minimal.img 
# minimal.img: DOS/MBR boot sector

# qemu:全系统模拟器
run: minimal.img
	qemu-system-x86_64 -monitor stdio $<

debug: minimal.img
	qemu-system-x86_64 -s -S $< &
	gdb -x init.gdb

clean:
	rm -f *.img

.PHONY: run debug
