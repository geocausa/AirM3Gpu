# J615 shared UMAPool selection/create transaction (E142)

E142 closes the lock/ownership transaction between E141's safe promotion state and the actual reusable shared Compute UMAPool owner, while keeping the path unreachable from live Queue creation.

Exact 23J220 `AGXChannel::init()` takes `AGXUMASharedPoolContainer +0x40`, reads the selected weak slot, and attempts the nonzero-only try-retain. If that fails, the same lock remains held across `halNewUMAPool()`, UMAPool initialization with shared/async flags, nested accelerator-list insertion, weak-slot publication and the container retain. The container lock is released only immediately before the channel's direct pool pointer is published. A correct replacement path therefore cannot unlock between “no promotable pool” and global pool-ID-consuming construction.

Linux now stores the actual `G15SharedComputeUmaPoolOwner` only as physical backing for an explicit logical lifetime state machine. The slot itself contributes no logical retain. Channel-direct and active-epoch counts remain authoritative; final zero removes the owner under the client-container mutex and drops it only after unlocking. This is intentionally not a naïve strong `Option<Pool>` lifetime.

`select_or_create_compute_channel()` holds the client-container mutex for the complete promotion-or-create decision and invokes the caller-supplied pool constructor while the transaction remains locked. Existing live slots are promoted without invoking construction. `G15ClientUmaComputeChannelRef::with_pool()` exposes the owner only after locked identity/liveness validation.

The per-VM container still has no Queue accessor/caller, and the shared-pool constructor remains uncalled by live code. No UMAPool, FList or global pool ID is therefore created/consumed by E142 itself, and no RunCompute producer changes.

Linux checkpoint: `594d39b1b85e35ae315d048d60528cfe9a8b0d10`, tree `1a469e6be6435ff56048209f298d57dc458f45dc`.
