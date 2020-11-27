resource "azurerm_subnet" "subnet" {
  name                 = var.name
  resource_group_name  = var.resourceGroupName
  virtual_network_name = var.virtualNetworkName
  address_prefixes     = var.addressPrefixes
  service_endpoints    = var.serviceEndpoints
}