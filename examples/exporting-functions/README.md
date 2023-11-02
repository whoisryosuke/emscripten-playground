# Notes

In order to run the function, you'll need to access the WASM `Module` exposed once you `require()` or import the WASM JS.

In the pre-generated Emscripten code, you can see they use the `Module` to create a progress loader. I usually comment this out and use my own script to access the function directly. We use the `Module.cwrap()` method to grab our exported C++ function and use it.

```html
<script type="text/javascript">
  var Module = {};
  Module["onRuntimeInitialized"] = function () {
    console.log("wasm loaded ");

    const int_sqrt = Module.cwrap(
      "int_sqrt", // name of C function
      "number", // return type
      ["number"] // argument types
    );

    console.log("Module.ccall", Module.ccall); // make sure it's not undefined
    // console.log("Module.int_sqrt", Module.int_sqrt); // make sure it's not undefined
    // console.log("Module.int_sqrt()", Module.int_sqrt()); // this should work
    console.log("cwrap", int_sqrt);
    console.log("int_sqrt", int_sqrt(12));
  };
</script>
```
