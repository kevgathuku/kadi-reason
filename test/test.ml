open OUnit2
open Lib.Poker

let test_start_game =
  "test suite for Poker"
  >::: [
         ( "starting with less players than allowed" >:: fun _ ->
           assert_equal 0
             (transition (Start 0) initialState :> Lib.Poker.state).num_players;
           assert_equal NotStarted
             (transition (Start 0) initialState :> Lib.Poker.state).status );
         ( "starting with allowed number of players" >:: fun _ ->
           assert_equal Lobby
             (transition (Start 2) initialState :> Lib.Poker.state).status );
         ( "invalid action" >:: fun _ ->
           assert_equal NotStarted
             (transition DealPlayerCards initialState :> Lib.Poker.state).status
         );
       ]

let _ = run_test_tt_main test_start_game
