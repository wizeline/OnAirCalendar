variable "name" {
  type = string
  description = "Group name"
}

variable "resourceGroupName" {
  type = string
  description = "Resource group name"
}

variable "location" {
  type = string
  description = "Location"
}

variable "accountTier" {
  type        = string
  description = "Account tier"
  default     = "Standard"
}

variable "accountReplicationType" {
  type        = string
  description = "Account tier"
  default     = "GRS"
}

variable "environment" {
  type = string
  description = "Environment value to be used on tags"
}