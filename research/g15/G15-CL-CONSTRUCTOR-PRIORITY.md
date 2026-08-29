# G15 normal CL-channel constructor / priority source chain

E119 closes the exact 23J220 CL workqueue/channel constructor inputs used by the stock-empty Compute path without changing Linux source.

A new CL workqueue receives command-queue `+0x8a0` and the current CL-workqueue count `+0x890`. Matching IOGPU/AGX workqueue constructors preserve those as workqueue `+0x54/+0xe0`; `AGXCLChannel::init()` then receives them as its second/first integer arguments. E118 proves the first is stored at channel `+0x38`, so the Compute SKU `evctl_index` is exactly the CL-workqueue index. The first CL channel uses index 0; normal J615's second integer is exact `0x50`.

The channel priority setter tuple is also exact: context type is the IOGPU effective queue priority, integer argument is `2`, and QoS is the queue QoS only for effective priority 1, otherwise literal 2. The optional accelerator priority-remap table is exact-zero on normal J615, so the integer argument does not remap. Effective foreground/runtime priority and queue QoS remain host runtime inputs rather than guessed constants.

This is static ABI/lifetime work only. Selected channel-state QueueInfo ownership still must be modeled before `channel_state_fwva` can feed the SKU serializer. Live G15 submission remains fail-closed; no RunCompute was issued.
