# Finito

A very experimental [Finite-state Machine](https://en.wikipedia.org/wiki/Finite-state_machine) simulator.

The [main.fn](./main.fn) file contains the syntax for describing the FSM.
The syntax is 

```lua
start := <STARTING_STATE>
accept := <ACCEPTING_STATE>

[ <STATE> | _ ] => [ <INPUT_TOKEN> | _ ] [ <OUTPUT_TOKEN> | _ ] <NEXT_STATE>
...
```

- `<STATE>` = `_` means to match the rule for every state
- `<INPUT_TOKEN>` = `_` means to match any input for that state
- `<OUTPUT_TOKEN>` = `_` means to not output any token for that state
- Note that `<NEXT_STATE>` cannot be `_` as that would imply setting the next state to every possible state

## Building and Running

```console
make run
```
