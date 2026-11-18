File locations:

- `package.json` (root) — orchestration scripts and shared devDependencies only; always `"private": true`.
- `turbo.json` (root) — the task pipeline definition.
- `pnpm-workspace.yaml` (root) — workspace globs.
- `apps/<name>/package.json` — one deployable per directory.
- `packages/<name>/package.json` — one shared library per directory.
- Shared tooling lives in `packages/config` (or `tooling/*`) and is consumed via `extends`.

Naming conventions:

- Scope all internal packages under a single npm scope, e.g. `@azaadi/ui`, `@azaadi/config`, `@azaadi/utils`.
- Use kebab-case for package directory names (`apps/web-app`, `packages/ui-kit`).
- Name turbo tasks after the npm script they run (`build`, `dev`, `lint`, `test`, `typecheck`).
- Reference internal dependencies with the `workspace:*` protocol, never a hard-coded version.

Package.json conventions per workspace member:


{
  "name": "@azaadi/ui",
  "version": "0.0.0",
  "private": true,
  "main": "./src/index.ts",
  "scripts": {
    "build": "tsup src/index.ts --format esm,cjs --dts",
    "lint": "eslint src/",
    "typecheck": "tsc --noEmit"
  }
}


Task pipeline conventions (`turbo.json`):

- Use `"dependsOn": ["^build"]` so a task waits for its internal dependencies.
- Declare every task's `outputs` explicitly so caching can restore artifacts.
- Mark long-running tasks with `"cache": false` and `"persistent": true` (e.g., `dev`).
- List environment variables a task reads under `"env"` so cache keys invalidate correctly.

Error handling:

- Turborepo fails fast by default; a non-zero exit in any package aborts the run and propagates the exit code to CI.
- Use `--continue` only when you intentionally want all packages attempted despite failures.
- Keep task scripts thin; push real logic into the package so failures surface with clear, localized stack traces.
- Treat cache misses as normal, not errors; never rely on side effects that only happen on a fresh build.