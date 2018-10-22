

module Html = Dom_html;
/* State declaration */

type position = {
  offsetX: float,
  offsetY: float
};
  
type state = {
  line: list(float),
  isPainting: bool,
  prevPos: position,
  count: int,
  myCanvasRef: ref(option(Dom.element))
};


/* Action declaration */
type action =
  | MouseDown
  | MouseMove
  | MouseUp
  | MouseLeave
  | PaintEnd;

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Paint");

let onMouseMove = (state) => ReasonReact.Update({...state, count: state.count + 1});
let onMouseDown = (state) => ReasonReact.Update({...state, count: state.count - 100});
let onMouseUp = (state) => ReasonReact.Update({...state, count: state.count + 1});
let onMouseLeave = (state) => ReasonReact.Update({...state, count: state.count + 1});
let setCanvasRef = (theRef, {ReasonReact.state}) => {
  state.myCanvasRef := Js.Nullable.toOption(theRef);
};



let canvas_id = "canvas";
let canvas =
  switch (Dom_html.getElementById(Dom_html.document, canvas_id)) {
  | None =>
    print_endline("cant find canvas " ++ canvas_id ++ " \n");
    failwith("fail");
  | Some(el) => Dom_html.elementToCanvasElement(el)
};
let context = Dom_html.canvasElementToJsObj(canvas)##getContext("2d");


/* greeting and children are props. `children` isn't used, therefore ignored.
   We ignore it by prepending it with an underscore */
   let make = ( _children ) => {
    /* spread the other default fields of component here and override a few */
    ...component,
  
    initialState: () => {count: 0, line: [0.0], isPainting: false, prevPos: {offsetX: 0.0, offsetY: 0.0}, myCanvasRef: ref(None)},
  
    /* State transitions */
    reducer: (action, state) =>
      switch (action) {
      /* | Click => ReasonReact.Update({...state, count: state.count + 1})
      | Toggle => ReasonReact.Update({...state, show: ! state.show}) */
      | MouseMove => onMouseMove(state)
      | MouseDown => onMouseDown(state)
      | MouseUp => onMouseUp(state)
      | MouseLeave => onMouseLeave(state)
      },

      didMount: (self) => {
        self.canvas.width = 1000;
        ReasonReact.NoUpdate
      },
  
    render: self => {
      let message = "You've clicked this " ++ string_of_int(self.state.count) ++ " times(s)";
    


      <div>
        <button onMouseMove=(_event => self.send(MouseMove))>
          (ReasonReact.string(message))
        </button>

        <canvas 
          id="canvas"
          ref={self.handle(setCanvasRef)}
          className="mainCanvas"
          onMouseDown=(_event => self.send(MouseDown))
          onMouseMove=(_event => self.send(MouseMove))
          onMouseUp=(_event => self.send(MouseUp))
          onMouseLeave=(_event => self.send(MouseLeave))>
        </canvas>
      </div>;
    },
  };