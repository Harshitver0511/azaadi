Prerequisites:

- Node.js `>=18.18` (match `.nvmrc`).
- pnpm `8.15+` — enable via Corepack: `corepack enable && corepack prepare pnpm@8.15.4 --activate`.

Initial setup:

bash
# from the repository root
git clone <azaadi-repo-url> && cd azaadi
corepack enable
pnpm install            # installs the whole workspace from the lockfile
pnpm turbo run build    # prime the local build cache


Environment variables:

- Copy the template and fill in per-package values: `cp .env.example .env`.
- Turbo tasks that read env vars must list them under `"env"` in `turbo.json` (or use `"globalEnv"` / `"globalPassThroughEnv"`) so cache keys stay correct.
- Turborepo-specific: `TURBO_TOKEN` and `TURBO_TEAM` enable remote caching; `TURBO_LOG_ORDER=stream` gives grouped, streaming logs.

Local Docker (optional, for reproducible CI or service dependencies):

dockerfile
# syntax=docker/dockerfile:1
FROM node:18.18-alpine AS base
RUN corepack enable
WORKDIR /app

FROM base AS deps
COPY pnpm-lock.yaml pnpm-workspace.yaml package.json ./
COPY apps ./apps
COPY packages ./packages
RUN pnpm install --frozen-lockfile

FROM deps AS build
COPY . .
RUN pnpm turbo run build


Common commands:

bash
# Dependency management
pnpm install                       # install all workspace deps
pnpm add <pkg> --filter @azaadi/ui # add a dep to one package
pnpm --filter "./apps/**" <script>  # run a script across a subset

# Build & orchestration
pnpm build                         # turbo run build (cached, topological)
pnpm dev                           # turbo run dev --parallel
pnpm turbo run build --filter=web  # build one app + its deps
pnpm turbo run build --dry=json    # preview the task graph without running

# Quality gates
pnpm lint                          # turbo run lint
pnpm test                          # turbo run test
pnpm typecheck                     # turbo run typecheck

# Caching
rm -rf .turbo node_modules/.cache/turbo  # clear local turbo cache
pnpm turbo run build --force           # ignore cache, rebuild everything


CI notes:

- Use `pnpm install --frozen-lockfile` in CI to guarantee lockfile integrity.
- Restore/populate the Turborepo cache (and set `TURBO_TOKEN`/`TURBO_TEAM` for remote cache) so unchanged packages are skipped.
- Run `pnpm turbo run lint test build` so tasks execute in dependency order with maximum parallelism.