import ReactReconciler, { type EventPriority } from "react-reconciler";
import { loadWASM } from "../integrations/wasm-module";

// Init WASM
let wasmModule: WebAssembly.Exports;

const rootHostContext = {};
const childHostContext = {};

const hostConfig: ReactReconciler.HostConfig<
  string,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any,
  any
> = {
  // now: Date.now,
  getRootHostContext: () => {
    return rootHostContext;
  },
  prepareForCommit: () => null,
  resetAfterCommit: () => {},
  clearContainer: () => {},
  getChildHostContext: () => {
    return childHostContext;
  },
  shouldSetTextContent: (type, props) => {
    return (
      typeof props.children === "string" || typeof props.children === "number"
    );
  },
  /**
 This is where react-reconciler wants to create an instance of UI element in terms of the target. Since our target here is the DOM, we will create document.createElement and type is the argument that contains the type string like div or img or h1 etc. The initial values of domElement attributes can be set in this function from the newProps argument
  */
  createInstance: (
    type: string,
    newProps: any,
    rootContainerInstance: any,
    _currentHostContext: any,
    workInProgress: any
  ) => {
    console.log("Creating instance", type);
    console.log("WASM module available?", wasmModule);
    const domElement = document.createElement(type);
    Object.keys(newProps).forEach((propName) => {
      const propValue = newProps[propName];
      if (propName === "children") {
        if (typeof propValue === "string" || typeof propValue === "number") {
          // @ts-ignore
          domElement.textContent = propValue;
        }
      } else if (propName === "onClick") {
        domElement.addEventListener("click", propValue);
      } else if (propName === "className") {
        domElement.setAttribute("class", propValue);
      } else {
        const propValue = newProps[propName];
        domElement.setAttribute(propName, propValue);
      }
    });
    return domElement;
  },
  createTextInstance: (text: string) => {
    return document.createTextNode(text);
  },
  appendInitialChild: (parent: any, child: any) => {
    parent.appendChild(child);
  },
  appendChild(parent: any, child: any) {
    parent.appendChild(child);
  },
  finalizeInitialChildren: (domElement: any, type: string, props: any) => true,
  supportsMutation: true,
  appendChildToContainer: (parent: any, child: any) => {
    parent.appendChild(child);
  },
  commitUpdate(
    domElement: any,
    updatePayload: any,
    type: string,
    oldProps: any,
    newProps: any
  ) {
    Object.keys(newProps).forEach((propName) => {
      const propValue = newProps[propName];
      if (propName === "children") {
        if (typeof propValue === "string" || typeof propValue === "number") {
          domElement.textContent = propValue;
        }
      } else {
        const propValue = newProps[propName];
        domElement.setAttribute(propName, propValue);
      }
    });
  },
  commitMount: () => {},
  commitTextUpdate(textInstance: any, oldText: string, newText: string) {
    textInstance.text = newText;
  },
  removeChild(parentInstance: any, child: any) {
    parentInstance.removeChild(child);
  },
  supportsPersistence: false,
  getPublicInstance: function (instance: any) {
    throw new Error("getPublicInstance Function not implemented.");
  },
  preparePortalMount: function (containerInfo: any): void {
    throw new Error("preparePortalMount Function not implemented.");
  },
  scheduleTimeout: function (
    fn: (...args: unknown[]) => unknown,
    delay?: number
  ) {
    throw new Error("scheduleTimeout Function not implemented.");
  },
  cancelTimeout: function (id: any): void {
    throw new Error("cancelTimeout Function not implemented.");
  },
  noTimeout: undefined,
  isPrimaryRenderer: false,
  getCurrentUpdatePriority: function (): ReactReconciler.Lane {
    throw new Error("getCurrentUpdatePriority Function not implemented.");
  },
  getInstanceFromNode: function (
    node: any
  ): ReactReconciler.Fiber | null | undefined {
    throw new Error("getInstanceFromNode Function not implemented.");
  },
  beforeActiveInstanceBlur: function (): void {
    throw new Error("beforeActiveInstanceBlur Function not implemented.");
  },
  afterActiveInstanceBlur: function (): void {
    throw new Error("afterActiveInstanceBlur Function not implemented.");
  },
  prepareScopeUpdate: function (scopeInstance: any, instance: any): void {
    throw new Error("prepareScopeUpdate Function not implemented.");
  },
  getInstanceFromScope: function (scopeInstance: any) {
    throw new Error("getInstanceFromScope Function not implemented.");
  },
  detachDeletedInstance: function (node: any): void {
    throw new Error("detachDeletedInstance Function not implemented.");
  },
  supportsHydration: false,
  NotPendingTransition: undefined,
  HostTransitionContext: undefined,
  setCurrentUpdatePriority: function (
    newPriority: ReactReconciler.EventPriority
  ): void {
    throw new Error("setCurrentUpdatePriority Function not implemented.");
  },
  resolveUpdatePriority: function (): ReactReconciler.EventPriority {
    console.log("resolveUpdatePriority Function not implemented.");
    return 0;
  },
  resetFormInstance: function (form: any): void {
    throw new Error("resetFormInstance Function not implemented.");
  },
  requestPostPaintCallback: function (callback: (time: number) => void): void {
    throw new Error("requestPostPaintCallback Function not implemented.");
  },
  shouldAttemptEagerTransition: function (): boolean {
    throw new Error("shouldAttemptEagerTransition Function not implemented.");
  },
  trackSchedulerEvent: function (): void {
    throw new Error("trackSchedulerEvent Function not implemented.");
  },
  resolveEventType: function (): null | string {
    throw new Error("resolveEventType Function not implemented.");
  },
  resolveEventTimeStamp: function (): number {
    throw new Error("resolveEventTimeStamp Function not implemented.");
  },
  maySuspendCommit: function (type: string, props: any): boolean {
    throw new Error("maySuspendCommit Function not implemented.");
  },
  preloadInstance: function (type: string, props: any): boolean {
    throw new Error("preloadInstance Function not implemented.");
  },
  startSuspendingCommit: function (): void {
    throw new Error("startSuspendingCommit Function not implemented.");
  },
  suspendInstance: function (type: string, props: any): void {
    throw new Error("suspendInstance Function not implemented.");
  },
  waitForCommitToBeReady: function ():
    | ((
        initiateCommit: (...args: unknown[]) => unknown
      ) => (...args: unknown[]) => unknown)
    | null {
    throw new Error("waitForCommitToBeReady Function not implemented.");
  },
};
// Init React reconciler
const ReactReconcilerInst = ReactReconciler(hostConfig);

const renderer = {
  init: async () => {
    // Init WASM
    wasmModule = await loadWASM();

    // wasmModule.init_gl()
  },
  render: (reactElement: any, domElement: any, callback: any) => {
    // Create a root Container if it doesnt exist
    if (!domElement._rootContainer) {
      // @ts-ignore
      domElement._rootContainer = ReactReconcilerInst.createContainer(
        domElement,
        false
      );
    }

    // update the root Container
    return ReactReconcilerInst.updateContainer(
      reactElement,
      domElement._rootContainer,
      null,
      callback
    );
  },
};

export default renderer;
