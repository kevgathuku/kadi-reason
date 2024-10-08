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
  | J
  | Q
  | K
  | A;

type card = {
  suit,
  number: cardValue,
};

// Game constants
let min_players = 2;
let max_players = 5;
let cards_to_deal = 4;

let start_cards_blocklist: list(cardValue) = [K, Q, J, A, Two, Three, Eight];
let finish_cards_blocklist: list(cardValue) = [K, Q, J, Two, Three, Eight];

type deck = list(card);

type player = {
  cards: deck,
  name: string,
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
  num_players: int,
  players: list(player),
  deck: list(card),
  played_stack: list(card),
  player_turn: int,
};

let initialState = {
  status: NotStarted,
  num_players: 0,
  players: [],
  deck: [],
  played_stack: [],
  player_turn: 0,
};

let contains = (element, list) => {
  List.exists(x => x == element, list);
};

let get_first_card = (~deck: list(card), ~blocklist) => {
  List.find(card => !contains(card.number, blocklist), deck);
};

let suits = [Hearts, Diamonds, Spades, Flowers];

let values = [
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  J,
  Q,
  K,
  A,
];

let cartesianProduct = (suits, values) => {
  List.concat(
    List.map(x => {List.map(y => {suit: x, number: y}, values)}, suits),
  );
};

let create_deck = () => cartesianProduct(suits, values);

let transition = (action, state: state) =>
  switch (state.status) {
  | NotStarted =>
    switch (action) {
    | Start(num_players)
        when num_players >= min_players && num_players <= max_players => {
        ...state,
        status: Lobby,
        num_players,
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
        List.length(updated_players) < min_players
        || List.length(updated_players) > max_players
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
    | DealPlayerCards => {...state, status: AwaitingStartCard}
    | _ => state
    }
  | AwaitingStartCard =>
    switch (action) {
    | DealStartCard =>
      let start_card =
        get_first_card(~deck=state.deck, ~blocklist=start_cards_blocklist);
      let remaining_deck = List.filter(x => x != start_card, state.deck);
      {
        ...state,
        status: Live,
        deck: remaining_deck,
        played_stack: [start_card],
      };
    | _ => state
    }
  | Live =>
    switch (action) {
    | PlayHand => {...state, status: Kadi}
    | _ => state
    }
  | Kadi =>
    switch (action) {
    | PlayHand => {...state, status: GameOver}
    | _ => state
    }
  | GameOver => state
  };
