import wasmModule from "./react-architecture-es6";

export async function loadWASM() {
  console.log("loading WASM");
  try {
    const instance = await wasmModule();
    console.log("WASM instance", instance);
    return instance;
  } catch (error) {
    console.error("Error loading WASM module:", error);
  }
}
