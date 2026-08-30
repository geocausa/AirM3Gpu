# J615 lazy Compute-channel retained context (E144)

E144 retains the remaining exact allocator/shared-bank handles required by a future lazy G15 Compute-channel transaction, but still invokes no pool/channel owner constructor.

E138 proved command-queue and bare workqueue construction create no TA/3D/CL channel or UMAPool. E143 then retained the per-client shared-pool container through Linux Queue lifetime. E144 additionally keeps the client range-5 uncached/cached allocator handles and cloneable shared-bank-1/q22 mapping-notifier handles in private G15 `QueueInner` fields.

Those are the exact context classes required by the dormant owners: the shared Compute UMAPool/FList owner needs cached range-5 plus bank1/q22 and device-global UMA state; the channel/command owner needs uncached range-5 plus bank1/q22 and manager-global Compute statistics.

No selector or constructor is called, no global pool ID is consumed, and `queue/compute.rs` remains unchanged. The future activation point is narrowed to actual Compute command preparation rather than eager generic `Queue::new()`/`WorkQueue::new()`.

Linux checkpoint: `6c65dc3a73c6c724d198015b6addfcf3bdfce5f3`, tree `c62064dd8399232215e5fb8328ead9061548e522`.
