import React from "react";
import wasmModule from "./react-architecture-es6";

type Props = {};

const RenderModule = (props: Props) => {
  async function loadWASM() {
    console.log("loading WASM");
    try {
      const instance = await wasmModule();
      console.log("WASM instance", instance);
      return instance;
    } catch (error) {
      console.error("Error loading WASM module:", error);
    }
  }

  return <div>RenderModule</div>;
};

export default RenderModule;
