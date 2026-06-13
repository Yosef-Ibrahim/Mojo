# Pro Tips

## Code Organization

```
src/
├─ managers/      (ThemeManager, AudioManager, ScoreManager)
├─ components/    (GameBoard, PlayerSetup, Settings, Leaderboard)
├─ hooks/         (useTheme, useAudio, useAnimation)
├─ games/         (GameEvaluator, MinimaxAI, OpeningBook)
└─ styles/        (CSS variables, animations)
```

## Testing Strategy

1. **Unit Test** — Single function works
2. **Integration Test** — Feature works with app
3. **End-to-End Test** — Full user journey works
4. **Regression Test** — Didn't break anything

## Git Workflow

```bash
git commit -m "Backup before improvements"     # Before starting
git add . && git commit -m "WIP: [feature]"    # During feature
git commit -m "Add [feature name]"             # After feature
git push origin main                            # Final
```

## Performance Tips

- Cache/memoize expensive calculations
- Use requestAnimationFrame for animations
- Lazy load non-essential components
- Minimize re-renders in React
- Test on real devices
- Monitor memory usage

## Debugging Tips

1. Check browser console (F12)
2. Read error messages carefully
3. Add console.log strategically
4. Test in isolation (single feature)
5. Compare code with guide examples
6. Search StackOverflow for error

## Learning Resources

### UX Dev
- [React Docs](https://react.dev)
- [CSS Animations (MDN)](https://developer.mozilla.org/en-US/docs/Web/CSS/animation)
- [Web Audio API (MDN)](https://developer.mozilla.org/en-US/docs/Web/API/Web_Audio_API)
- [Responsive Design (web.dev)](https://web.dev/responsive-web-design-basics/)
- [W3C Accessibility](https://www.w3.org/WAI/fundamentals/)

### AI Dev
- [Minimax Algorithm](https://en.wikipedia.org/wiki/Minimax)
- [Game Theory Basics](https://en.wikipedia.org/wiki/Game_theory)
- [Alpha-Beta Pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)
- [Negamax Algorithm](https://en.wikipedia.org/wiki/Negamax)
- [Chess Programming](https://www.chessprogramming.org/)

### Tools
- [VS Code](https://code.visualstudio.com/)
- [Chrome DevTools](https://developer.chrome.com/docs/devtools/)
- [Lighthouse](https://developers.google.com/web/tools/lighthouse) — Performance audit
- [WebAIM](https://webaim.org/) — Accessibility
- [Excalidraw](https://excalidraw.com/) — Planning diagrams
