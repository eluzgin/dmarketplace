<h1 class="contract">addproduct</h1>

---
spec_version: "0.2.0"
title: Add product
summary: 'Add a new product'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{author}} to add a new product information to products table.
Product information will include a name of product {{product_name}}, and short description {{product_description}}, 
price {{price}}, in which currency {{currency}}, thumbnail url {{thumbnail}} and full image url {{image}}.

<h1 class="contract">updproduct</h1>

---
spec_version: "0.2.0"
title: Update product
summary: 'Update product'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{author}} to update current product information.

<h1 class="contract">addcategory</h1>

---
spec_version: "0.2.0"
title: Add category
summary: 'Add category'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{author}} to add a new category of products.

<h1 class="contract">updcategory</h1>

---
spec_version: "0.2.0"
title: Update category
summary: 'Update category'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{author}} to update existing category.

<h1 class="contract">listproduct</h1>

---
spec_version: "0.2.0"
title: List Product for sale
summary: 'List Product for sale'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{seller}} to list a product he authored for sale. 


<h1 class="contract">addpayaddr</h1>

---
spec_version: "0.2.0"
title: Add payment address for seller
summary: 'Add payment address'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{seller}} to register acceptable currency {{currency}} 
and corresponding public address {{address}} for payment.

<h1 class="contract">createorder</h1>

---
spec_version: "0.2.0"
title: Create order
summary: 'Create order'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{buyer}} to create a new order for a product {{product_id}}.

<h1 class="contract">marksold</h1>

---
spec_version: "0.2.0"
title: Mark order sold
summary: 'Mark order sold'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{author}} to mark order id {{order_id}} as sold.

<h1 class="contract">addreview</h1>

---
spec_version: "0.2.0"
title: Add review
summary: 'Add actor review'
icon: @ICON_BASE_URL@/@SMART_CONTRACT_ACTION_ICON_URI@
---

This action's purpose is for {{reviewer}} to provide review of {{actor}} with rank {{rank}} and comments {{comments}}.
