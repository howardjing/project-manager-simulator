
type complexity = Small | Medium | Large;
type state = NotStarted | InProgress | NeedsQA | Completed;

type comment = {
  content: string,
};

type ticket = {
  fromSprint: int,
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
  fromSprint: 0,
  title,
  content,
  complexity,
  state: NotStarted,
  parent: None,
  comments: [],
};

let generateTicket = (~fromSprint, ~complexity, ~parent) => {
  ...makeAncestorTicket(
    ~title=switch(parent) {
      | Some(ticket) => "Expanding on " ++ ticket.title
      | None => "Generated Ticket " ++ string_of_int(fromSprint)
    },
    ~content="While doing this sprint, we've decided to do some more work.",
    ~complexity,
  ),
  fromSprint,
  parent,
}

let isComplete: list(ticket) => bool = List.for_all((ticket) => ticket.state == Completed);

Random.self_init();

/**
 * Generate a random number between 0 and 1. If the given rate is lower than the generated number,
 * return true. Otherwise return false.
 */
let sometimes = (rate: float): bool => Random.float(1.0) < rate;

let sometimesSpawnMoreTickets = (~fromSprint, ~parent) => {
  if (sometimes(0.1)) {
    [generateTicket(~fromSprint=fromSprint, ~complexity=Small, ~parent)]
  } else if (sometimes(0.05)) {
    [generateTicket(~fromSprint=fromSprint, ~complexity=Medium, ~parent)]
  } else if (sometimes(0.05)) {
    [generateTicket(~fromSprint=fromSprint, ~complexity=Large, ~parent)]
  } else {
    []
  }
}

let work = (points: (complexity) => int, velocity: int, tickets: list(ticket), sprintCount: int): list(ticket) => {

  let workDone = List.fold_left(((processed, pointsDone), ticket) => {
    let nextPointsDone = pointsDone + points(ticket.complexity);
    let moreDiscoveredWork = sometimesSpawnMoreTickets(~fromSprint={sprintCount}, ~parent=Some(ticket))

    /* we've reached our limit, or the ticket was already finished, or sometimes we don't feel like doing something, so don't do work */
    if (nextPointsDone > velocity || ticket.state == Completed || sometimes(0.2) ) {
      ([ticket, ...processed], pointsDone)
    } else {
      let workedOnTicket = {
        ...ticket,
        state: Completed,
      };
      (List.concat([[workedOnTicket, ...processed], moreDiscoveredWork]), nextPointsDone)
    }
  }, ([], 0), tickets) |> fst |> List.rev;

  let moreIndependentWork = sometimesSpawnMoreTickets(~fromSprint=sprintCount, ~parent=None)

  List.concat([workDone, moreIndependentWork])
};

/**
 * entry function that does work
 */
let rec doWork = (points: (complexity) => int, velocity: int, currentSprint: list(ticket), previousSprints: list(list(ticket))): list(list(ticket)) => {
  if (isComplete(currentSprint)) {
    [currentSprint, ...previousSprints]
  } else {
    doWork(points, velocity, work(points, velocity, currentSprint, List.length(previousSprints)), [currentSprint, ...previousSprints])
  }
}

let makeChildTicket = (~title, ~content, ~complexity, ~parent) => { ...makeAncestorTicket(~title, ~content, ~complexity), parent }

let totalPoints = (points: (complexity) => int, tickets: list(ticket)): int => {
  List.fold_left((sum, ticket) => { sum + points(ticket.complexity) }, 0, tickets)
}

let naiveEstimate = (points: (complexity) => int, velocity: int, tickets: list(ticket)): int => {
  float_of_int(totalPoints(points, tickets)) /. float_of_int(velocity) |> ceil |> int_of_float
}

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

let ticket12 = makeAncestorTicket(
  ~title="Add ability to estimate completion time of tickets",
  ~content={| For now a rough estimate. Estimation will be in terms of number of sprints. A sprint will have a baseline velocity. Once velocity is
    hit, no further tickets will be processed. Tickets will have a certain probability of completion based on complexity. If a ticket fails to complete,
    it is rolled over into the next sprint. Failed tickets have a probability to decrement in complexity. Iterate until no more tickets.
  |},
  ~complexity=Large,
)

let ticket13 = makeAncestorTicket(
  ~title="Add ability to randomly generate futher tickets",
  ~content={| To better estimate project complexity, assume that tickets have a certain probability to generate further tickets. These tickets are added to the
    backlog.
  |},
  ~complexity=Large,
)

let ticket14 = makeChildTicket(
  ~title="Figure out how to use hashmaps",
  ~content={| Can't find a simple to follow example of using a hashmap. |},
  ~complexity=Small,
  ~parent=Some(ticket1),
)

let ticket15 = makeChildTicket(
  ~title="Figure out how to convert ints to string",
  ~content={| We can only concat strings with strings -- figure out how to concat a string with a number. |},
  ~complexity=Small,
  ~parent=Some(ticket1),
)

let ticket16 = makeAncestorTicket(
  ~title="Compute naive estimate of project",
  ~content="Given a team's velocity and total number of points, estimate the number sprints it will take to complete the project.",
  ~complexity=Medium,
)

let ticket17 = makeChildTicket(
  ~title="Display naive estimate of project",
  ~content="Display the naive estimate of a project.",
  ~complexity=Small,
  ~parent=Some(ticket16)
)

let points = (complexity): int => {
  switch(complexity) {
    | Small => 2
    | Medium => 3
    | Large => 5
  }
}

let ourVelocity = 5

let tickets = [ticket1, ticket2, ticket3, ticket4, ticket5, ticket6, ticket7, ticket8, ticket9, ticket10, ticket11, ticket12, ticket13, ticket14, ticket15, ticket16, ticket17];
let project = doWork(points, ourVelocity, tickets, []) |> List.rev;
