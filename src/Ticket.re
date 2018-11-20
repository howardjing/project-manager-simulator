
type complexity = Small | Medium | Large;
type state = NotStarted | InProgress | NeedsQA | Completed;

type comment = {
  content: string,
};

type ticket = {
  title: string,
  content: string,
  complexity,
  state,
  parent: option(ticket),
  comments: list(comment),
};

let complexityToString = (complexity) => switch (complexity) {
  | Small => "small"
  | Medium => "medium"
  | Large => "large"
}

let stateToString = (state) => switch (state) {
  | NotStarted => "not started"
  | InProgress => "in progress"
  | NeedsQA => "needs qa"
  | Completed => "completed"
}

let makeAncestorTicket = (~title, ~content, ~complexity) => {
  title,
  content,
  complexity,
  state: NotStarted,
  parent: None,
  comments: [],
};

let makeChildTicket = (~title, ~content, ~complexity, ~parent) => { ...makeAncestorTicket(~title, ~content, ~complexity), parent }

let ticket1 = makeAncestorTicket(
  ~title="Learn basics of Reason ML",
  ~content="Learn basics of Reason ML. To get started, just the syntax is sufficient.",
  ~complexity=Medium,
);


let ticket2 = makeChildTicket(
  ~title="Learn basics of ReasonReact",
  ~content="Let's learn ReasonReact so we can start making Project Manager Simulator! For now, just the basics are sufficient.",
  ~complexity=Medium,
  ~parent=Some(ticket1),
);

let ticket3 = makeAncestorTicket(
  ~title="Render list of tickets",
  ~content="Let's show our list of tickets!",
  ~complexity=Medium,
);

/* Turns out default args are not what I thought they were -- they must be optional. It seems like the best thing to do is have multiple functions */
let ticket4 = {
  ...makeChildTicket(
    ~title="Figure out how to make a function with default args",
    ~content="I'd like to default a ticket's state to `NotStarted`, and parent to `None`. Figure out how to make a helper function that returns a `ticket` that assigns `ticket` some default values.",
    ~complexity=Small,
    ~parent=Some(ticket1),
  ),
  state: InProgress,
  comments: [
    {
      content: {|
        My original intention was to figure out how to add default arguments to a function. This way we could prefill in our function with some sane defaults. It looks like this is not
        supported in Reason ML the way I want it to. Instead of defaulting to some value if they're not passed in, default arguments can be omitted, but they must be an
        option. When a default argument is omitted, they default to `None`.

        The solution I wound up going with was to have a couple of differently named functions that return tickets. The `makeAncestorTicket` creates a working ticket with sane defaults.
        When I need to override a specific property, I can use use `makeAncestorTicket` to create a base record with sane defaults, then use the update record syntax to modify the properties directly.
        Since there are so many tickets who only differ in their parent ticket, I make a `makeChildTicket` to handle this commin use case. This seems like a good compromise.
      |}
    },
  ]
};

let ticket5 = {
  ...makeChildTicket(
    ~title="Figure out how to display a string in ReasonReact",
    ~content="I'd like to display a ticket's title. It looks like `div` elements do not accept strings. Instead, we need to convert our string into a ReasonReact.reactElement.",
    ~complexity=Small,
    ~parent=Some(ticket2),
  ),
  state: Completed,
};

let ticket6 = makeChildTicket(
  ~title="Figure out how to convert a variant to a string",
  ~content={|
    A ticket's complexity and state are currently modelled as variants. To render them within a div, it looks like I need to first convert a variant to a string.
    Ideally there'd be a quick way to do this. For now, I can manually write functions that convert variants to strings.
  |},
  ~complexity=Small,
  ~parent=Some(ticket1),
);

let ticket7 = makeChildTicket(
  ~title="Figure out how to insert non breaking space",
  ~content="I'd like to figure out how to render a non breaking space in ReasonReact. Just inserting &nbsp; doesn't seem to work.",
  ~complexity=Small,
  ~parent=Some(ticket2),
);

let ticket8 = makeAncestorTicket(
  ~title="Add ability to create new ticket",
  ~content="I'd like to have the ability to create a new ticket. Currently the only tickets that show up have been hardcoded in.",
  ~complexity=Medium,
);

let ticket9 = makeAncestorTicket(
  ~title="Add ability to edit a ticket",
  ~content={| I'd like to be able to update a ticket's title / content / complexity / state / parent once it's been created. |},
  ~complexity=Large,
);

let ticket10 = makeAncestorTicket(
  ~title="Render tickets in columns",
  ~content="Instead of rendering our list of tickets as an unordered list, I'd like a column view where each column represents the ticket's state.",
  ~complexity=Medium,
);

let ticket11 = {
  ...makeAncestorTicket(
    ~title="Add ability to comment on ticket",
    ~content="Add ability to add a comment to a ticket.",
    ~complexity=Medium,
  ),
  state: Completed,
}


let tickets = [ticket1, ticket2, ticket3, ticket4, ticket5, ticket6, ticket7, ticket8, ticket9, ticket10, ticket11];
