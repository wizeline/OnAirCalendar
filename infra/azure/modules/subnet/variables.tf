variable "name" {
  type = string
  description = "Group name"
}

variable "resourceGroupName" {
  type = string
  description = "Resource group name"
}

variable "virtualNetworkName" {
  type = string
  description = "Virtual network name"
}

variable "addressPrefixes" {
  type        = list(string)
  description = "Address prefixes"
}

variable "serviceEndpoints" {
  type        = list(string)
  description = "Service endpoints"
}

variable "environment" {
  type = string
  description = "Environment value to be used on tags"
}