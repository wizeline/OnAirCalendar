resource "azurerm_virtual_network" "vnet" {
  name                = var.name
  address_space       = var.addressSpace
  location            = var.location
  resource_group_name = var.resourceGroupName

  tags = {
      Name          = var.name
      Environment   = var.environment
      Terraform     = true
  }
}
