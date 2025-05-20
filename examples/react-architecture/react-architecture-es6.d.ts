// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
declare namespace RuntimeExports {
    let createContext: any;
}
interface WasmModule {
  _add_ui(_0: number): void;
  _init_gl(): void;
  _change_color(_0: number): void;
  _render_gl(): void;
}

interface EmbindModule {
}

export type MainModule = WasmModule & typeof RuntimeExports & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
