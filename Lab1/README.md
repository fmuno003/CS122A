# Part 1 Single synchSM

Bouncing LED: Using RIBS, capture a synchSM that lights the LEDs one-at-a-time in sequence B0, then B1, then B2, ..., then B7, then B0 again. Each LED stays on for 1 second. Next, have A0 select the sequence direction: 0 as above, 1 in reverse.

Pattern Sequencer: Next, create any sequence of 10 output patterns, such as 11111111, 01010101, 10101010, ... (ten such patterns). Store them in an array. Each pattern displays for 1 second. A0 being 0 goes through the pattern forwards, 1 backwards.

Music Accompaniment: Use a smart phone/tablet to play at least 20 seconds of a (clean) song of your choice, create a light show that accompanies the song, following the song in some way.

# Part 2 Concurrent synchSM's

Using at least three "concurrent" synchSMs, devise a system where A7A6 being 00 ewxecutes the above Bouncing LED synchSM, 01 the Pattern Sequencer, and 10 the Music Accompaniment. 11 simply outpues 11111111. While the synchSMs are concurrent, note that all but one will be waiting in a wait state at any time.

# Part 3 Creativity

Develop an idea for system wth at least two truly concurrent synchSMs. The more interesting the behavior, the better. Note that the concurrency need not only relate to output displays, but may also detect certain events/sequences on the inputs. Capture the system in RIBS and simulate.

# Part 4 Partner Challenge

Write s specification of your above system for a partner to implement. Write your names along with each spec, e.g. Create: Jonny Smith, Implementer: Marry Jones.

# Part 5 Challenge

Create a google doc that has a specification and diagrams for future people learning to program state machines. MAke it intersting and fun. Perhaps a game or a real application.
