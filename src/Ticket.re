
type complexity = Small | Medium | Large;
type state = NotStarted | InProgress | NeedsQA | Completed;

type ticket = {
  title: string,
  content: string,
  complexity,
  state,
  parent: option(ticket),

};

let ticket1 = {
  title: "Learn Basics of Reason ML",
  content: "Learn basics of Reason ML. To get started, just the syntax is sufficient.",
  complexity: Medium,
  state: NotStarted,
  parent: None,
};

let ticket2 = {
  title: "Learn Basics of ReasonReact",
  content: "Let's learn ReasonReact so we can start making Project Manager Simulator! For now, just the basics is sufficient.",
  complexity: Medium,
  state: NotStarted,
  parent: Some(ticket1),
};

let tickets = [ticket1, ticket2];
