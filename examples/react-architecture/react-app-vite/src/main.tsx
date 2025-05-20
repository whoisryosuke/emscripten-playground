import React from "react";
import ReactDOM from "react-dom/client";
import "./index.css";
import App from "./App";
import CustomRenderer from "./renderer/renderer";

// const root = ReactDOM.createRoot(
//   document.getElementById('root') as HTMLElement
// );
const rootElement = document.getElementById("root");
const renderUI = async () => {
  await CustomRenderer.init();

  CustomRenderer.render(
    <React.StrictMode>
      <App />
    </React.StrictMode>,
    rootElement,
    () => console.log("Finished Rendering UI or something")
  );
};

renderUI();
