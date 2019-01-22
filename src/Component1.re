/* This is the basic component. */
let component = ReasonReact.statelessComponent("Page");

/* Your familiar handleClick from ReactJS. This mandatorily takes the payload,
   then the `self` record, which contains state (none here), `handle`, `reduce`
   and other utilities */
let handleClick = (_event, _self) => Js.log("clicked!");

let renderSprint = (tickets: list(Ticket.ticket), state: Ticket.state) => {

  let stateStyle = ReactDOMRe.Style.make(
    ~fontSize="1.5em",
    ~textTransform="capitalize"
  , ());

  <div>
    <div style=(stateStyle)>
      (Ticket.stateToString(state) |> ReasonReact.string)
    </div>
    (
      List.map((ticket: Ticket.ticket) =>
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
          (if (List.length(ticket.comments) == 0) { ReasonReact.null } else {
            <>
            <h4>(ReasonReact.string("Comments:"))</h4>
            <ul>
              (
                List.map((comment: Ticket.comment) =>
                  <li>(ReasonReact.string(comment.content))</li>
                , ticket.comments)
                |> Array.of_list
                |> ReasonReact.array
              )
            </ul>
            </>
          })
        </div>
      , tickets)
        |> Array.of_list
        |> ReasonReact.array
    )
  </div>
}

let totalPoints = Ticket.totalPoints(Ticket.points)
let naiveEstimate = Ticket.naiveEstimate(Ticket.points)

/* `make` is the function that mandatorily takes `children` (if you want to use
   `JSX). `message` is a named argument, which simulates ReactJS props. Usage:

   `<Page message="hello" />`

   Which desugars to

   `ReasonReact.element(Page.make(~message="hello", [||]))` */



let gridContainer = ReactDOMRe.Style.make(
  ~display="grid",
  ~gridTemplateColumns="repeat(" ++ string_of_int(List.length(Ticket.states)) ++ ", 1fr)",
  ()
);

let make = (~message, _children) => {
  ...component,
  render: self =>
    <div>
      <div>(ReasonReact.string("Initial (naive) estimate | Total Points: " ++ string_of_int(totalPoints(Ticket.tickets)) ++ ", Sprints: " ++ string_of_int(naiveEstimate(Ticket.ourVelocity, Ticket.tickets))))</div>
      <div>
        (
          List.mapi((i: int, sprint: list(Ticket.ticket)) =>
            <div>
              <h1>(ReasonReact.string("Sprint " ++ string_of_int(i + 1) ++ ":"))</h1>
              <div style=(gridContainer)>
                  (
                    Ticket.states |>
                      List.map((state: Ticket.state) =>
                        renderSprint(List.filter((ticket: Ticket.ticket) => ticket.state == state, sprint), state)
                      ) |> Array.of_list |> ReasonReact.array)
              </div>
            </div>
          , Ticket.project
          ) |> Array.of_list
            |> ReasonReact.array
        )
      </div>
    </div>,
};
