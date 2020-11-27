provider "azurerm" {
  version = ">=2.34.0"
  features {}
}

module "resource_group" {
  source            = "./../modules/resource_group"
  name              = "oac-rg-${var.env}"
  environment       = var.env
  location          = var.location
}
