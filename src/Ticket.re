
type complexity = Small | Medium | Large;
type state = NotStarted | InProgress | NeedsQA | Completed;

type ticket = {
  title: string,
  content: string,
  complexity,
  state,
  parent: option(ticket),

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

let ticket1 = {
  title: "Learn basics of Reason ML",
  content: "Learn basics of Reason ML. To get started, just the syntax is sufficient.",
  complexity: Medium,
  state: NotStarted,
  parent: None,
};

let ticket2 = {
  title: "Learn basics of ReasonReact",
  content: "Let's learn ReasonReact so we can start making Project Manager Simulator! For now, just the basics are sufficient.",
  complexity: Medium,
  state: NotStarted,
  parent: Some(ticket1),
};

let ticket3 = {
  title: "Render list of tickets",
  content: "Let's show our list of tickets!",
  complexity: Medium,
  state: NotStarted,
  parent: None,
}

let ticket4 = {
  title: "Figure out how to make a function with default args",
  content: "I'd like to default a ticket's state to `NotStarted`, and parent to `None`. Figure out how to make a helper function that returns a `ticket` that assigns `ticket` some default values.",
  complexity: Small,
  state: InProgress,
  parent: Some(ticket1),
}

let ticket5 = {
  title: "Figure out how to display a string in ReasonReact",
  content: "I'd like to display a ticket's title. It looks like `div` elements do not accept strings. Instead, we need to convert our string into a ReasonReact.reactElement.",
  complexity: Small,
  state: Completed,
  parent: Some(ticket2),
}

let ticket6 = {
  title: "Figure out how to convert a variant to a string",
  content: {|
    A ticket's complexity and state are currently modelled as variants. To render them within a div, it looks like I need to first convert a variant to a string.
    Ideally there'd be a quick way to do this. For now, I can manually write functions that convert variants to strings.
  |},
  complexity: Small,
  state: NotStarted,
  parent: Some(ticket1),
}

let ticket7 = {
  title: "Figure out how to insert non breaking space",
  content: "I'd like to figure out how to render a non breaking space in ReasonReact. Just inserting &nbsp; doesn't seem to work.",
  complexity: Small,
  state: NotStarted,
  parent: Some(ticket2),
}



let tickets = [ticket1, ticket2, ticket3, ticket4, ticket5, ticket6, ticket7];
