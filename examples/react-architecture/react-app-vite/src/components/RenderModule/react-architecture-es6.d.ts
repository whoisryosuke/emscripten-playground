declare interface Module {
  _add(a: number, b: number): number;
  add(a: number, b: number): number; // Using cwrap
  cwrap: typeof cwrap;
  onRuntimeInitialized: () => void;
}
