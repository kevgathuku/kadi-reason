type suit =
  | Hearts
  | Diamonds
  | Spades
  | Flowers;

type cardValue =
  | Two
  | Three
  | Four
  | Five
  | Six
  | Seven
  | Eight
  | Nine
  | Ten
  | K
  | Q
  | J
  | A;

type card = {
  suit,
  number: string,
};

type deck = list(card);

type player = {
  cards: deck,
  name: string,
};

type gameRules = {
  start_cards_blocklist: list(cardValue),
  finishing_cards: list(cardValue),
  num_players: int,
  cards_to_deal: int,
};

type action =
  | Start(int)
  | AddPlayer(string)
  | AddDeck(deck)
  | DealPlayerCards
  | DealStartCard
  | PlayHand
  | Pick
  | Finish;

type gameStatus =
  | NotStarted
  | Lobby
  | AwaitingDeck
  | AwaitingPlayerCards
  | AwaitingStartCard
  | Live
  | Kadi
  | GameOver;

type state = {
  status: gameStatus,
  rules: gameRules,
  players: list(player),
  deck: list(card),
  played: list(card),
  player_turn: int,
};

let defaultRules = {
  start_cards_blocklist: [K, Q, J, A, Two, Three, Eight],
  finishing_cards: [A, Two, Three, Four, Five, Six, Seven, Nine, Ten],
  num_players: 2,
  cards_to_deal: 4,
};

let initialState = {
  status: NotStarted,
  rules: defaultRules,
  players: [],
  deck: [],
  played: [],
  player_turn: 0,
};

let transition = (action, state: state) =>
  switch (state.status) {
  | NotStarted =>
    switch (action) {
    | Start(num_players) when num_players >= 2 => {
        ...state,
        status: Lobby,
        rules: {
          ...state.rules,
          num_players,
        },
      }
    | _ => state
    }
  //   Add checks for player names in state, dups, etc.
  | Lobby =>
    switch (action) {
    | AddPlayer(player_name) =>
      // TODO: Add handling for existing player
      let updated_players = {
        let new_player = {name: player_name, cards: []};
        let existing_players = state.players;
        List.rev([new_player, ...existing_players]);
      };
      let new_status =
        List.length(updated_players) < state.rules.num_players
          ? Lobby : AwaitingDeck;

      {...state, players: updated_players, status: new_status};

    | _ => state
    }
  | AwaitingDeck =>
    switch (action) {
    | AddDeck(new_deck) => {
        ...state,
        deck: new_deck,
        status: AwaitingPlayerCards,
      }
    | _ => state
    }
  | AwaitingPlayerCards =>
    switch (action) {
    | DealPlayerCards => 
    {
        ...state,
        status: AwaitingStartCard,
      }
    | _ => state
    }
  | AwaitingStartCard =>
    switch (action) {
    | DealStartCard => Live
    | _ => state
    }
  | Live =>
    switch (action) {
    | PlayHand => Kadi
    | _ => state
    }
  | Kadi =>
    switch (action) {
    | PlayHand => GameOver
    | _ => state
    }
  | GameOver => state
  };
