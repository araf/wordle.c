# WORDLE.C — A Minimal Wordle Clone in C
#### Video Demo:  <URL HERE>

---

## Description

**WORDLE.C** is a console-based clone of the popular Wordle puzzle, written entirely in C11 using only the standard library and ANSI escape sequences for colors. The game runs in the terminal, randomly selecting a five-letter word as the hidden answer. Players have six attempts to guess the word, and after each guess, feedback is shown with colored letters.

This project demonstrates string manipulation, file I/O, randomization, terminal display, and structured game design in C. The aim is to keep the code minimal while still recreating a fun, playable version of Wordle.

---

## File Structure

```
.
├── wordle.c                 # Main program file
├── README.md                # Documentation
└── wordlist/
    ├── selected_words.txt   # Candidate answers
    └── all_words.txt        # Allowed guesses
```

**`wordle.c`** implements the entire game loop: word selection, user input, dictionary validation, color-coded display, and win/lose detection.
**`selected_words.txt`** contains words that may be chosen as the answer.
**`all_words.txt`** lists all valid guesses. The program validates every guess against this file.

---

## Gameplay

- The answer is always a five-letter word.
- You have **six attempts**.
- Feedback is color coded:
  - **Red**: incorrect letter.
  - **Green**: correct letter in the correct place.
  - **Blue**: correct letter in the wrong place.

The board updates after each guess, showing guesses so far. Win by guessing within six attempts; otherwise the program reveals the answer.

---

## How It Works

1. **Random answer selection**: `get_random_word()` counts lines in `selected_words.txt`, picks a random index, and returns that word.
2. **Input handling**: Guesses are collected with `scanf("%5s", buffer)`. Extra characters are discarded.
3. **Validation**: `word_found()` scans `all_words.txt` to confirm guesses are valid words.
4. **Feedback**: The program first marks exact matches (green), then checks remaining letters for misplaced matches (blue). Others remain red.
5. **Display**: ANSI escape codes are used to color the output. The screen clears each turn using `system("clear")` (Linux/macOS) or `system("cls")` (Windows).

---

## Wordlist Format

- `selected_words.txt`: lowercase, 5 letters per line, newline-terminated.
- `all_words.txt`: also 5 letters plus newline per entry. The program reads 6-byte records (`5 chars + '
'`).

**Important:** Ensure LF (`
`) line endings, not CRLF (`
`). On Windows, convert using tools like `dos2unix`.

---

## Build & Run

### Compile
```bash
gcc -std=c11 -Wall -Wextra -O2 wordle.c -o wordle
```

### Run (Play)
```bash
./wordle
# or
./wordle play
```

### Run (Debug Mode)
```bash
./wordle debug apple
```
Debug mode sets the answer manually. The word must appear in `all_words.txt`.

---

## Design Choices

- **ANSI colors**: Portable across modern terminals.
- **Screen clearing**: Simple system calls (`cls`/`clear`). Could be replaced with ANSI `\033[2J\033[H`.
- **Two-pass marking**: Greens first, then misplaced blues, avoids double-counting.
- **Static buffer**: Used in `get_random_word()` to keep return values valid.
- **Strict dictionary check**: Ensures valid guesses only.

---

## Limitations

- **File formatting**: Relies on exact `5 letters + newline`. Inconsistent line endings may cause issues.
- **Debug mode mutation**: The answer pointer references `argv[2]`; modifying it later could be unsafe. Copying into a buffer would be safer.
- **Platform dependency**: Relies on `system("clear")`/`system("cls")`. Some terminals may not behave as expected.

---

## Future Enhancements

- “Word of the day” mode using deterministic seeding.
- Statistics tracking (streaks, wins).
- Hash table dictionary for faster validation.
- Option for traditional Wordle colors (grey/yellow/green).
- Replay prompt instead of immediate exit.

---

## Troubleshooting

- **Colors not showing**: Ensure ANSI is supported. On Windows, use PowerShell or Windows Terminal.
- **Rejected words**: Ensure your word exists in `all_words.txt` with LF endings.
- **No clear screen**: Replace system calls with ANSI clear sequences.

---

## Credits

- Program design and logic by me.
- Word selection routine inspired by examples of random file line retrieval.
- Wordlists adapted for this project.

---

## Conclusion

This project shows how a popular puzzle can be recreated in a few hundred lines of C. It demonstrates file handling, randomization, validation, and text-based UI with colors. Despite its simplicity, it provides an engaging and faithful Wordle-like experience in the terminal.
