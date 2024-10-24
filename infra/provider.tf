provider "aws" {
  region  = "us-east-1"  # Use your preferred region
}

provider "aws" {
  alias   = "route53"
  region  = "us-east-1"  # Region for Route 53 (if you are using a custom domain)
}
