# https://medium.com/@gmusumeci/how-to-create-an-azure-remote-backend-for-terraform-67cce5da1520
resource "azurerm_resource_group" "group" {
  name     = var.name
  location = var.location

  tags = {
      Name          = var.name
      Environment   = var.environment
      Terraform     = true
  }
}
