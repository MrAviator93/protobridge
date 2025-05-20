# Contributing

Contributions, issues, and feature requests are welcome! Feel free to check issues page. For major changes, please open an issue first to discuss what you would like to change.

## Branch Naming Conventions

When creating a new branch, please use the following naming conventions:

- For new features: feature/your-new-feature (e.g., feature/i2c-error-handling)
- For bug fixes: bugfix/your-bug-fix (e.g., bugfix/temperature-reading-error)
- For hotfixes: hotfix/your-hotfix (e.g., hotfix/memory-leak-fix)

Please ensure that the branch names are plural, reflecting that they may include multiple changes or updates related to the same feature or fix.

## Commit Message Guidelines

A good commit message should be clear and concise, yet descriptive enough to understand the change without having to look at the code. Here's a suggested format for commit messages:

1. Subject Line: This should be a brief summary of the change, ideally not exceeding 50 characters. It should be written in imperative mood, as if commanding the codebase to change. Example: "Add temperature conversion feature."

2. Body: Following a blank line after the subject, the body should provide a detailed description of the change. You should explain the problem being addressed, the changes made to address it, and any other relevant information. The body is wrapped at 72 characters.

3. Footer: This section is used for referencing issue tracker IDs and giving credits.

## Example of a Commit Message

```txt
Add support for BMP180 sensor

- Implement BMP180 read functionality in I2C library.
- Update documentation to include BMP180 usage instructions.
- Add unit tests for BMP180 sensor readings.

Resolves: #123
Credit: @MrAviator93 for initial implementation
```
