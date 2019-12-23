<h1 class="contract">adduser</h1>

---
spec_version: "0.2.0"
title: Add User
summary: 'Add a User'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{manager}} to add {{user}} as a user. User's description will be {{description}}.

<h1 class="contract">addmanager</h1>

---
spec_version: "0.2.0"
title: Add Manager
summary: 'Add a Manager'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{$action.account}} to add {{manager}} as a manager. Manager's description will be {{description}}.

<h1 class="contract">rmuser</h1>

---
spec_version: "0.2.0"
title: Remove User
summary: 'Remove a User'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{manager}} to remove {{user}} as a user.

<h1 class="contract">rmmanager</h1>

---
spec_version: "0.2.0"
title: Remove Manager
summary: 'Remove a Manager'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{$action.account}} to remove {{manager}} as a manager.

<h1 class="contract">logproduct</h1>

---
spec_version: "0.2.0"
title: Log Product
summary: 'Log Product'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{logger}} to log a new product into the system. Product will have a tag number of {{product_tag}} usually associated with RFID encoded id on physical product, name of product {{product_name}}, and short description {{product_description}}.


<h1 class="contract">logrecord</h1>

---
spec_version: "0.2.0"
title: Log Record
summary: 'Log Record'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{logger}} to log any associated record for a product {{product_id}}.


<h1 class="contract">setproducttag</h1>

---
spec_version: "0.2.0"
title: Set Product Tag
summary: 'Set Product Tag'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{logger}} to set the product tag id {{product_tag}} to {{new_product_tag}}.

<h1 class="contract">sold</h1>

---
spec_version: "0.2.0"
title: Sold
summary: 'Sold'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{logger}} to mark product {{product_id}} as sold.
