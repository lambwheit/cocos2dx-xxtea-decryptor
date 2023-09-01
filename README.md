# cocos2dx-xxtea-decryptor
cocos2dx xxtea source decryptor (LUAC, JSC and more)

Release binary was built in x86 mode

Usage: exeName.exe [key] [sign] [input file/dir]

How to find xxtea key:

Search for decrypt function in the cocos2dx binary, and hook using frida, 3rd argument is normally the key

How to find xxtea sign:

open multiple encrypted files in a text editor, they should have a common string at the start of the file, that is the sign\n
