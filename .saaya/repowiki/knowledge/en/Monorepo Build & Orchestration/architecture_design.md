The monorepo follows a layered, dependency-directed architecture with three tiers:

1. Root orchestration layer (`/package.json`, `/turbo.json`, `/pnpm-workspace.yaml`): Owns workspace membership, shared tooling versions, and the task graph. It contains no application code and is always `private`.
2. Application layer (`apps/*`): Deployable units such as web frontends, API services, and background workers. Apps consume packages but are never consumed by them.
3. Shared package layer (`packages/*`): Reusable libraries (UI components, config presets, shared types, utilities). Packages may depend on other packages but never on apps, keeping the graph acyclic.

Dependency flow is strictly one-directional: `apps -> packages -> packages`. Turborepo enforces this at build time via the `^` (topological) prefix in `dependsOn`, guaranteeing that a package's internal dependencies are built before the package itself. This prevents the diamond/circular dependency problems that plague loosely managed multi-repo setups.

Integration pattern — task pipeline as a DAG:


        build (root task)
       /                \
  build:web          build:api
       |                  |
  ^build (deps)       ^build (deps)
       \                  /
        shared packages (ui, config, utils)


Data flow and caching: Turborepo computes a hash from a task's source files, its declared `inputs`, environment variables, and the hashes of its dependency tasks. On a cache hit it replays the terminal output and restores the declared `outputs` (e.g., `dist/**`, `.next/**`) from the local cache (`.turbo/cache`) or a configured remote cache. On a miss it executes the task and writes results back to the cache. This makes the pipeline deterministic and incrementally reproducible.

Configuration layering: shared TypeScript, ESLint, and Prettier presets live in `packages/config` (or `tooling/*`) and are referenced by each app/package via `extends`. This centralizes conventions while allowing per-package overrides, and it is itself a workspace package so it participates in the same build graph.