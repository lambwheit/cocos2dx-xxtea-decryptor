# cocos2dx-xxtea-decryptor
cocos2dx xxtea source decryptor (LUAC, JSC and more)

Release binary was built in x86 mode

Usage: exeName.exe [key] [sign] [input file/dir]

How to find xxtea key:

Search for decrypt function in the cocos2dx binary, and hook using frida, 3rd argument is normally the key

How to find xxtea sign:

open multiple encrypted files in a text editor, they should have a common string at the start of the file, that is the sign


**EXAMPLE FRIDA SCRIPT TO DUMP XXTEA KEY**

```js
var module_name_libcocos2dlua_so='libcocos2dlua.so';  //change libcocos2dlua.so to the correct binary name

function start_timer_for_intercept() {
  setTimeout(
    function() {
      var offset_of_xxtea_decrypt_00a5be08=0x95be08;//update offset of decrypt function to the correct one <- can be found using ghidra
      var dynamic_address_of_xxtea_decrypt_00a5be08=Module.findBaseAddress(module_name_libcocos2dlua_so).add(offset_of_xxtea_decrypt_00a5be08);
      Interceptor.attach(dynamic_address_of_xxtea_decrypt_00a5be08, {
                 onEnter: function(args) {
                    console.log("Entered xxtea_decrypt_00a5be08");
                    console.log("[key]-> " + args[2].readCString()) // print XXTEA key 
                    //console.log('args[0]='+args[0]+' , args[1]='+args[1]+' , args[2]='+args[2]+' , args[3]='+args[3]+' , args[4]='+args[4]);
                    // this.context.x0=0x1;
                  },
                  onLeave: function(retval) {
                    //console.log("Exited xxtea_decrypt_00a5be08, retval:"+retval);
                    // retval.replace(0x1);
                  }
       });
    }, 2000);//milliseconds
}
start_timer_for_intercept();

```
