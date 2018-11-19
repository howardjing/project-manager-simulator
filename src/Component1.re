/* This is the basic component. */
let component = ReasonReact.statelessComponent("Page");

/* Your familiar handleClick from ReactJS. This mandatorily takes the payload,
   then the `self` record, which contains state (none here), `handle`, `reduce`
   and other utilities */
let handleClick = (_event, _self) => Js.log("clicked!");

/* `make` is the function that mandatorily takes `children` (if you want to use
   `JSX). `message` is a named argument, which simulates ReactJS props. Usage:

   `<Page message="hello" />`

   Which desugars to

   `ReasonReact.element(Page.make(~message="hello", [||]))` */
let make = (~message, _children) => {
  ...component,
  render: self =>
    <div>
      <div>(ReasonReact.string("Here are some tickets:"))</div>
      <ul>
        (
          List.map((ticket: Ticket.ticket) =>
            <li>
              <div>
                <h3>(ReasonReact.string(ticket.title))</h3>
                <div>
                  <span>
                    (ticket.complexity |> Ticket.complexityToString |> ReasonReact.string)
                    (ReasonReact.string(" | "))
                    (ticket.state |> Ticket.stateToString |> ReasonReact.string)
                  </span>
                </div>
                <div>(ReasonReact.string(ticket.content))</div>
              </div>
            </li>
          , Ticket.tickets)
            |> Array.of_list
            |> ReasonReact.array
        )
      </ul>
      <div onClick=(self.handle(handleClick))>
        (ReasonReact.string(message))
      </div>
    </div>,
};
