Core orchestration and package management:

- **pnpm** (`>=8.15`) — workspace package manager; uses a symlinked, content-addressable store for fast, disk-efficient installs.
- **Turborepo** (`turbo@^2.x`) — task runner and build cache; reads `turbo.json` for the pipeline.
- **Node.js** (`>=18.18`, pinned via `.nvmrc` / `engines`) — runtime for tooling and scripts.

Language and static analysis:

- **TypeScript** (`^5.4`) — shared compiler; base config in `packages/tsconfig`.
- **ESLint** (`^9`) + **Prettier** (`^3`) — linting and formatting, configured via shared presets.

Typical root `package.json` shape:


{
  "name": "azaadi",
  "private": true,
  "packageManager": "pnpm@8.15.4",
  "engines": { "node": ">=18.18.0" },
  "scripts": {
    "build": "turbo run build",
    "dev": "turbo run dev --parallel",
    "lint": "turbo run lint",
    "test": "turbo run test",
    "typecheck": "turbo run typecheck",
    "format": "prettier --write \"**/*.{ts,tsx,js,json,md}\""
  },
  "devDependencies": {
    "turbo": "^2.0.0",
    "typescript": "^5.4.0",
    "prettier": "^3.2.0",
    "eslint": "^9.0.0"
  }
}


Workspace declaration (`pnpm-workspace.yaml`):

yaml
packages:
  - "apps/*"
  - "packages/*"


Version constraint conventions:

- Pin the package manager with the `packageManager` field so all contributors and CI use the same pnpm.
- Use caret ranges (`^`) for libraries to allow compatible minor/patch updates.
- Use pnpm `workspace:*` protocol for internal cross-package dependencies so they always resolve to the local version:


{
  "dependencies": {
    "@azaadi/ui": "workspace:*",
    "@azaadi/config": "workspace:*"
  }
}
