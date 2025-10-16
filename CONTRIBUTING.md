# Contributing to Flow

Thank you for your interest in contributing to Flow! 🎉

## Getting Started

1. **Fork the repository**
2. **Clone your fork:**
   ```bash
   git clone https://github.com/anonymodgh/Flow.git
   cd Flow
   ```
3. **Build Flow:**
   ```bash
   make
   ```
4. **Run tests:**
   ```bash
   make test
   ```

## Development Setup

### Requirements
- g++ with C++17 support
- Python 3.x
- Node.js
- Git

### Building
```bash
make clean
make
```

### Testing
```bash
# Run all tests
make test

# Run specific test
./flow examples/test.fl

# Check metrics
./flow metrics
```

## How to Contribute

### Reporting Bugs
- Use GitHub Issues
- Include Flow version (`flow --version`)
- Provide minimal reproducible example
- Include error messages and stack traces

### Suggesting Features
- Open a GitHub Issue with `[Feature Request]` prefix
- Describe the use case
- Provide example code if possible

### Submitting Code

1. **Create a branch:**
   ```bash
   git checkout -b feature/my-feature
   ```

2. **Make your changes:**
   - Follow existing code style
   - Add tests for new features
   - Update documentation

3. **Test your changes:**
   ```bash
   make clean
   make
   make test
   ```

4. **Commit:**
   ```bash
   git commit -m "Add feature: description"
   ```

5. **Push and create PR:**
   ```bash
   git push origin feature/my-feature
   ```

## Code Style

### C++ (flow.cpp)
- Use 4 spaces for indentation
- Follow existing naming conventions
- Add comments for complex logic
- Keep functions focused and small

### Documentation
- Update README.md for user-facing changes
- Add examples for new features
- Update CHANGELOG.md

### Examples (.fl files)
- Include comments explaining the code
- Keep examples simple and focused
- Test examples before submitting

## Areas for Contribution

### High Priority
- [ ] Improve C++ parallel execution
- [ ] Add more integration examples
- [ ] Improve error messages
- [ ] Performance optimizations

### Medium Priority
- [ ] Add more language support (Rust, Go)
- [ ] Improve debugging tools
- [ ] Add more macros
- [ ] Better Windows support

### Low Priority
- [ ] GUI for Flow
- [ ] VSCode extension
- [ ] Package registry
- [ ] Hot reload

## Testing Guidelines

### Adding Tests
1. Create a new `.fl` file in `examples/`
2. Test should be self-contained
3. Include expected output in comments
4. Add to `make test` if appropriate

### Test Categories
- **Unit tests:** Test individual features
- **Integration tests:** Test feature combinations
- **Performance tests:** Benchmark performance
- **Error tests:** Test error handling

## Documentation

### What to Document
- New features
- Breaking changes
- Bug fixes
- Performance improvements

### Where to Document
- `README.md` - User-facing features
- `CHANGELOG.md` - All changes
- `docs/` - Detailed guides
- Code comments - Implementation details

## Release Process

1. Update `CHANGELOG.md`
2. Update version in `flow.cpp`
3. Run full test suite
4. Create release tag
5. Build binaries for all platforms
6. Update documentation

## Community

- Be respectful and inclusive
- Help others learn
- Share your Flow projects
- Provide constructive feedback

## Questions?

- Open a GitHub Issue
- Check existing documentation
- Look at examples in `examples/`

---

**Thank you for contributing to Flow!** 🚀
