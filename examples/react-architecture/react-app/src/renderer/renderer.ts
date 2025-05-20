import ReactReconciler from "react-reconciler";

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
  prepareUpdate(domElement: any, oldProps: any, newProps: any) {
    return true;
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
    throw new Error("Function not implemented.");
  },
  preparePortalMount: function (containerInfo: any): void {
    throw new Error("Function not implemented.");
  },
  scheduleTimeout: function (
    fn: (...args: unknown[]) => unknown,
    delay?: number
  ) {
    throw new Error("Function not implemented.");
  },
  cancelTimeout: function (id: any): void {
    throw new Error("Function not implemented.");
  },
  noTimeout: undefined,
  isPrimaryRenderer: false,
  getCurrentEventPriority: function (): ReactReconciler.Lane {
    throw new Error("Function not implemented.");
  },
  getInstanceFromNode: function (
    node: any
  ): ReactReconciler.Fiber | null | undefined {
    throw new Error("Function not implemented.");
  },
  beforeActiveInstanceBlur: function (): void {
    throw new Error("Function not implemented.");
  },
  afterActiveInstanceBlur: function (): void {
    throw new Error("Function not implemented.");
  },
  prepareScopeUpdate: function (scopeInstance: any, instance: any): void {
    throw new Error("Function not implemented.");
  },
  getInstanceFromScope: function (scopeInstance: any) {
    throw new Error("Function not implemented.");
  },
  detachDeletedInstance: function (node: any): void {
    throw new Error("Function not implemented.");
  },
  supportsHydration: false,
};
const ReactReconcilerInst = ReactReconciler(hostConfig);

const renderer = {
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
