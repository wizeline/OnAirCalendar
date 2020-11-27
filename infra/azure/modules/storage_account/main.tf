resource "azurerm_storage_account" "storage_account" {
  name                     = var.name
  resource_group_name      = var.resourceGroupName
  location                 = var.location
  account_tier             = var.accountTier
  account_replication_type = var.accountReplicationType

  tags = {
      Name          = var.name
      Environment   = var.environment
      Terraform     = true
  }
}
