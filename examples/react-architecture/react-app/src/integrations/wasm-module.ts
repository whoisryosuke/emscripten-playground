export default class WASMModule {
  loaded = false;
  modulePath = "";
  module: WebAssembly.Exports | null = null;

  constructor(newPath: string) {
    // Module["onRuntimeInitialized"] = function () {};
    this.modulePath = newPath;
  }

  async load() {
    try {
      const response = await fetch("/my_wasm_module.wasm");
      const bytes = await response.arrayBuffer();
      const results = await WebAssembly.instantiate(bytes);
      this.module = results.instance.exports;
    } catch (err) {
      console.error("Error loading wasm", err);
    }
  }
}
