provider "azurerm" {
  version = "=2.34.0"
  features {}
}

module "resource_group" {
  source            = "./../modules/resource_group"
  name              = "oac-backend-rg-${var.environment}"
  environment       = var.environment
  location          = var.location
}

module "virtual_network" {
  source            = "./../modules/virtual_network"
  name              = "oac-backend-vnet-${var.environment}"
  addressSpace      = ["10.0.0.0/16"]
  resourceGroupName = module.resource_group.name
  location          = var.location
  environment       = var.environment
}

module "subnet" {
  source                = "./../modules/subnet"
  name                  = "oac-backend-subnet-${var.environment}"
  resourceGroupName     = module.resource_group.name
  virtualNetworkName    = module.virtual_network.name
  addressPrefixes       = ["10.0.2.0/24"]
  serviceEndpoints      = ["Microsoft.Storage"]
  environment           = var.environment
}

module "storage_account" {
  source                = "./../modules/storage_account"
  name                  = "oacbackstoacc${var.environment}"
  resourceGroupName     = module.resource_group.name
  location              = var.location
  accountTier           = "Standard"
  accountReplicationType = "GRS"
  environment           = var.environment
}

module "storage_container" {
  source              = "./../modules/storage_container"
  name                = "oac-backend-stocont-${var.environment}"
  containerAccessType = "blob"
  storageServiceName  = module.storage_account.name
}
